#include "SystemManager.h"

#include "DependencyException.h"
#include "../util/ostream_helper.h"
#include "../util/Map.h"

using engine::util::Map;

#include <iostream>
#include <algorithm>
#include <list>

namespace engine {
    namespace ECS {
        SystemManager::SystemManager(EntityManager& em) : em(em), threads(this->numThreads()) {}
        SystemManager::~SystemManager() {
            this->enabledSystems.clear();
        }
        
        size_t SystemManager::numThreads() const {
            return std::thread::hardware_concurrency();
        }
            
        void SystemManager::setup() {
            if(!this->checkDependencySatisfaction()) {
                throw WTFException("You have unsatisfied System dependencies. Maybe you forgot to enable a System.");
            }
            auto roots = this->buildDependencyGraph();
            if(this->isGraphCircular(roots)) {
                throw WTFException("Your System dependencies are circular. This is forbidden!");
            }
            auto& em = this->em;
            auto& bq = this->queue;
            for(size_t i = 0; i < this->numThreads(); ++i) {
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
        }

        void SystemManager::stop() {
            for(size_t i = 0; i < this->numThreads(); ++i) {
                this->queue.push(std::make_unique<StopTask>());
            }
            for(size_t i = 0; i < this->numThreads(); ++i) {
                this->threads[i].join();
            }
        }

        void SystemManager::run() {
            // Traverse tree in breath first.
            // fringe is used as a queue.
            Map<SystemNode*, std::shared_future<void>> futures;
            futures.set_empty_key(nullptr);
            vector<shared_ptr<SystemNode>> fringe(this->enabledSystems.size());
            fringe.insert(fringe.end(), this->dependencyTree.begin(), this->dependencyTree.end());
            for(size_t fringeHead = 0; fringe.size() - fringeHead > 0; ++fringeHead) {
                auto& node = fringe[fringeHead];
                for(auto& child : node->children) {
                    fringe.push_back(child);
                }
                // Check for dependencies
                /* TODO: Resolve problem:
                 * This locks up everything if the parents have not been started yet!
                 */
                for(auto& parent : node->parents) {
                    // Wait for dependency to finish.
                    futures[parent.lock().get()].get();
                }
                auto promise = std::make_unique<std::promise<void>>();
                futures[node.get()] = std::move(promise->get_future().share());
                this->queue.push(std::make_unique<Task>(node->system, std::move(promise)));
            }
            // Wait for the rest to finish
            for(auto& pair : futures) {
                pair.second.get();
            }
        }
        
        vector<shared_ptr<SystemManager::SystemNode>> SystemManager::buildDependencyGraph() const {
            vector<shared_ptr<SystemNode>> roots;
            vector<unique_ptr<vector<systemId_t>>> optDeps;
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
            
            vector<shared_ptr<SystemNode>> allNodes;
            // Create all nodes and find roots.
            for(size_t i = 0; i < this->enabledSystems.size(); ++i) {
                auto sys = this->enabledSystems[i];
                auto node = std::make_shared<SystemNode>(sys);
                allNodes.push_back(node);
                if(sys->getDependencies().size() == 0 && optDeps[i]->size() == 0) {
                    roots.push_back(node);
                }
            }
            
            // Create all links.
            for(size_t i = 0; i < allNodes.size(); ++i) {
                auto requiredBy = allNodes[i];
                for(auto& dep : requiredBy->system->getDependencies()) {
                    for(auto dependency : allNodes) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                            requiredBy->parents.push_back(dependency);
                        }
                    }
                }
                for(auto& dep : *optDeps[i]) {
                    for(auto dependency : allNodes) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                            requiredBy->parents.push_back(dependency);
                        }
                    }
                }
            }
            
            return roots;
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

