#include "SystemManager.h"

#include "DependencyException.h"
#include "../util/ostream_helper.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <stdint.h>

namespace engine {
    namespace ECS {
        SystemManager::SystemManager(EntityManager& em) : em(em), threads(0), hasBeenSetup(false) {
            // Init numThreads with the number of cores.
            this->setNumberOfThreads(std::thread::hardware_concurrency());
        }
        
        SystemManager::~SystemManager() {
            this->enabledSystems.clear();
            this->dependencyTree.clear();
            this->queue.clear();
        }
        
        void SystemManager::setNumberOfThreads(size_t n) {
            if(this->hasBeenSetup) {
                throw WTFException("The SystemManager has already been setup!");
            }
            this->numThreads = n;
            this->threads = std::move(Array<std::thread>(n));
            this->queue.resize(n);
        }
            
        void SystemManager::setup() {
            if(this->hasBeenSetup) {
                throw WTFException("The SystemManager has already been setup!");
            }
            if(!this->checkDependencySatisfaction()) {
                throw WTFException("You have unsatisfied System dependencies. Maybe you forgot to enable a System.");
            }
            auto roots = this->buildDependencyGraph();
            if(this->isGraphCircular(roots)) {
                throw WTFException("Your System dependencies are circular. This is forbidden!");
            }
            this->assignLayers();
            auto& em = this->em;
            auto& bq = this->queue;
            for(size_t i = 0; i < this->numThreads; ++i) {
                auto t = std::thread([&]{
                    while(true) {
                        auto task = std::move(bq.pop());
                        if(!task->stop()) {
                            try {
                                task->system->run(em);
                            } catch(...) {
                                task->promise->set_exception(std::current_exception());
                            }
                            task->promise->set_value();
                        } else {
                            break;
                        }
                    }
                });
                t.detach();
                this->threads[i] = std::move(t);
            }
            this->hasBeenSetup = true;
        }

        void SystemManager::stop() {
            if(!this->hasBeenSetup) {
                throw WTFException("The SystemManager has not been setup yet!");
            }
            for(size_t i = 0; i < this->numThreads; ++i) {
                this->queue.push(std::make_unique<StopTask>());
            }
            for(size_t i = 0; i < this->numThreads; ++i) {
                this->threads[i].join();
            }
        }

        void SystemManager::run() {
#ifdef DEBUG
            if(this->hasBeenSetup) {
                throw WTFException("The SystemManager has already been setup!");
            }
#endif
            // This is used as a queue.
            vector<std::shared_future<void>> futures;
            futures.reserve(this->dependencyTree.size());
            size_t futuresHead = 0;
            
            size_t layer = this->dependencyTree[0]->layer;
            for(auto& node : this->dependencyTree) {
                if(layer != node->layer) {
                    // New layer => wait for all tasks to finish
                    while(futures.size() - futuresHead > 0 /*i.e. queue not empty*/) {
                        // Wait for future to be done.
                        // TODO: Exceptions that have been thrown in the worker thread
                        //       have been catched but will be thrown here again.
                        //       => catch and handle or at least log.
                        futures[futuresHead++].get();
                    }
                    layer = node->layer;
                }
                // Start all tasks
                auto promise = std::make_unique<std::promise<void>>();
                futures.push_back(std::move(promise->get_future().share()));
                this->queue.push(std::make_unique<Task>(node->system, std::move(promise)));
            }
            // Wait for the last layer to finish
            while(futures.size() - futuresHead > 0 /*i.e. queue not empty*/) {
                // Wait for future to be done.
                // TODO: Exceptions that have been thrown in the worker thread
                //       have been catched but will be thrown here again.
                //       => catch and handle or at least log.
                futures[futuresHead++].get();
            }
        }
        
        void SystemManager::traverse(const shared_ptr<SystemNode>& start, Set<SystemNode*>& visited) const {
            for(auto& node : start->children) {
                visited.insert(node.get());
                this->traverse(node, visited);
            }
        }
        
        vector<shared_ptr<SystemManager::SystemNode>> SystemManager::buildDependencyGraph() {
            vector<shared_ptr<SystemNode>> roots;
            vector<unique_ptr<vector<systemId_t>>> optDeps;
            optDeps.reserve(this->enabledSystems.size());
            for(auto& sys : this->enabledSystems) {
                auto dArr = sys->getOptionalDependencies();
                auto deps = std::make_unique<vector<systemId_t>>();
                deps->reserve(dArr.size());
                for(auto d : dArr) {
                    for(auto& s : this->enabledSystems) {
                        // Only keep the optional dependencies that can be
                        // satisfied.
                        if(s->getSystemTypeId() == d) {
                            deps->push_back(d);
                            break;
                        }
                    }
                }
                optDeps.push_back(std::move(deps));
            }
            
            dependencyTree.reserve(this->enabledSystems.size());
            // Create all nodes and find roots.
            for(size_t i = 0; i < this->enabledSystems.size(); ++i) {
                auto sys = this->enabledSystems[i];
                auto node = std::make_shared<SystemNode>(sys);
                this->dependencyTree.push_back(node);
                if(sys->getDependencies().size() == 0 && optDeps[i]->size() == 0) {
                    roots.push_back(node);
                }
            }
            
            // Create all links.
            for(size_t i = 0; i < this->dependencyTree.size(); ++i) {
                auto requiredBy = this->dependencyTree[i];
                for(auto& dep : requiredBy->system->getDependencies()) {
                    for(auto dependency : this->dependencyTree) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                            requiredBy->parents.push_back(dependency);
                        }
                    }
                }
                for(auto& dep : *optDeps[i]) {
                    for(auto dependency : this->dependencyTree) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                            requiredBy->parents.push_back(dependency);
                        }
                    }
                }
            }
            
            optDeps.clear();
            
            for(auto& node : this->dependencyTree) {
                for(auto it = node->children.begin(); it != node->children.end();) {
                    // Delete direct connection if there is a transitive connection
                    Set<SystemNode*> visited;
                    visited.set_empty_key(nullptr);
                    for(auto it2 = node->children.begin(); it2 != node->children.end(); ++it2) {
                        if((*it).get() == (*it2).get()) {
                            continue;
                        }
                        this->traverse(*it2, visited);
                    }
                    if(visited.find((*it).get()) != visited.end()) {
                        // Has another connection => delete the direct one
                        it = node->children.erase(it);
                    } else {
                         ++it;
                    }
                }
            }
            
            return roots;
        }
        
        bool SystemManager::isSubset(const std::vector<shared_ptr<SystemManager::SystemNode>>& left, const Set<SystemManager::SystemNode*>& right) const {
            for(auto& l : left) {
                if(right.find(l.get()) == right.end()) {
                    return false;
                }
            }
            return true;
        }
        
        void SystemManager::assignLayers() {
            // Using the Coffman-Graham Layering Algorithm.
            // Source: https://cs.brown.edu/~rt/gdhandbook/chapters/hierarchical.pdf
            
            size_t maxLayerWidth = this->numThreads;
            
            for(auto& node : this->dependencyTree) {
                node->layer = SIZE_MAX;
            }
            
            for(size_t i = 0; i < this->dependencyTree.size(); ++i) {
                shared_ptr<SystemNode> node;
                size_t minNumberParents = SIZE_MAX;
                for(auto& n : this->dependencyTree) {
                    if(n->layer != SIZE_MAX) {
                        continue;
                    }
                    if(minNumberParents > n->parents.size()) {
                        minNumberParents = n->parents.size();
                        node = n;
                    }
                }
                node->layer = i;
            }
                
            size_t k = 0;
            Set<SystemNode*> visited;
            visited.set_empty_key(nullptr);
            Set<SystemNode*> sumLayers;
            sumLayers.set_empty_key(nullptr);
            vector<vector<shared_ptr<SystemNode>>> layers;
            layers.push_back({});
            while(visited.size() < this->dependencyTree.size()) {
                shared_ptr<SystemNode> node;
                size_t maxLayer = 0;
                for(auto& n : this->dependencyTree) {
                    if(visited.find(n.get()) != visited.end()) {
                        continue;
                    }
                    if(!this->isSubset(n->children, visited)) {
                        continue;
                    }
                    if(maxLayer <= n->layer) {
                        maxLayer = n->layer;
                        node = n;
                    }
                }
                // Yes, <= *is* correct. Don't ask me why though.
                if(layers[k].size() <= maxLayerWidth && this->isSubset(node->children, sumLayers)) {
                    layers[k].push_back(node);
                } else {
                    // Update sumL
                    for(auto& n : layers[k]) {
                        sumLayers.insert(n.get());
                    }
                    ++k;
                    layers.push_back({node});
                }
                visited.insert(node.get());
            }
            
            for(size_t i = 0; i < layers.size(); ++i) {
                for(auto& node : layers[i]) {
                    node->layer = i;
                }
            }
            
            // As per definition of the algorithm the higher indexed layers need
            // to be executed first. Thus descending order.
            std::sort(this->dependencyTree.begin(), this->dependencyTree.end(),
                    [](const auto& l, const auto& r) { return l->layer > r->layer; });
        }
        
        bool SystemManager::__isGraphCircular(const shared_ptr<SystemNode>& node, vector<shared_ptr<SystemNode>> visited) const {
            if(std::find(visited.begin(), visited.end(), node) != visited.end()) {
                return true;
            }
            visited.push_back(node);
            for(auto& child : node->children) {
                if(this->__isGraphCircular(child, visited)) {
                    return true;
                }
            }
            return false;
        }
        
        bool SystemManager::isGraphCircular(const vector<shared_ptr<SystemNode>>& roots) const {
            if(roots.size() == 0) {
                // If it does not have a node without any dependencies it is a circle.
                return true;
            }
            for(auto& root : roots) {
                vector<shared_ptr<SystemNode>> visited;
                if(this->__isGraphCircular(root, visited)) {
                    return true;
                }
            }
            return false;
        }
        
        bool SystemManager::checkDependencySatisfaction() const {
            for(auto& sys : this->enabledSystems) {
                for(auto depId : sys->getDependencies()) {
                    bool satisfied = false;
                    for(auto& sysDep : this->enabledSystems) {
                        if(sysDep->getSystemTypeId() == depId) {
                            satisfied = true;
                            break;
                        }
                    }
                    if(!satisfied) {
                        return false;
                    }
                }
            }
            return true;
        }
    }
}

