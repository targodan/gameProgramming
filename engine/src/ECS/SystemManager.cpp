#include "SystemManager.h"

#include "DependencyException.h"
#include "../util/ostream_helper.h"

#include <iostream>
#include <algorithm>
#include <list>

namespace engine {
    namespace ECS {
        SystemManager::SystemManager() {}
        SystemManager::~SystemManager() {
            this->enabledSystems.clear();
            this->systemOrder.clear();
        }
        
        void SystemManager::computeRunOrder() {
            if(!this->checkDependencySatisfaction()) {
                throw DependencyException("Some enabled Systems have unsatisfied dependencies.");
            }
            vector<shared_ptr<depNode>> roots = this->buildDependencyGraph();
            if(this->isGraphCircular(roots)) {
                throw DependencyException("Your System dependencies are circular. This is not allowed!");
            }
            vector<shared_ptr<depNode>> visited;
            for(auto& root : roots) {
                this->systemOrder.push_back({});
                this->linearizeDependencyGraph(root, visited, this->systemOrder.size()-1);
            }
            // Remove empty lists.
            auto newEnd = std::remove_if(this->systemOrder.begin(), this->systemOrder.end(),
                    [](auto list) {return list.size() == 0;});
            this->systemOrder.erase(newEnd, this->systemOrder.end());
        }
        
        vector<shared_ptr<SystemManager::depNode>> SystemManager::buildDependencyGraph() const {
            vector<shared_ptr<depNode>> roots;
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
            
            vector<shared_ptr<depNode>> allNodes;
            // Create all nodes and find roots.
            for(size_t i = 0; i < this->enabledSystems.size(); ++i) {
                auto sys = this->enabledSystems[i];
                auto node = std::make_shared<depNode>(sys);
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
        
        vector<shared_ptr<System>> SystemManager::mergeDependencySublists(
                const vector<shared_ptr<System>>& v1,
                const vector<shared_ptr<System>>& v2) const {
            vector<shared_ptr<System>> ret;
            ret.reserve(v1.size() + v2.size());
            bool disjoint = true;
            auto oldIt1 = v1.begin();
            auto oldIt2 = v2.begin();
            for(auto it1 = v1.begin(); it1 != v1.end(); ++it1) {
                for(auto it2 = v2.begin(); it2 != v2.end(); ++it2) {
                    if(*it1 == *it2) {
                        if(oldIt2 > it2) {
                            throw WTFException("Elements are contained in different orders. Cannot merge!");
                        }
                        disjoint = false;
                        for(auto it = oldIt1; it != it1; ++it) {
                            ret.push_back(*it);
                        }
                        for(auto it = oldIt2; it != it2; ++it) {
                            ret.push_back(*it);
                        }
                        ret.push_back(*it1);
                        oldIt1 = it1+1;
                        oldIt2 = it2+1;
                    }
                }
            }
            if(disjoint) {
                throw WTFException("The two lists are disjoint. Cannot merge!");
            }
            for(auto it = oldIt1; it != v1.end(); ++it) {
                ret.push_back(*it);
            }
            for(auto it = oldIt2; it != v2.end(); ++it) {
                ret.push_back(*it);
            }
            return ret;
        }
        
        void SystemManager::dbg_printSystems() const {
            std::cout << this->systemOrder << std::endl;
        }
        
        void SystemManager::linearizeDependencyGraph(const shared_ptr<depNode>& root,
                vector<shared_ptr<depNode>>& visited,
                size_t listIndex) {
            if(std::find(visited.begin(), visited.end(), root) != visited.end()) {
                return;
            }
            // Basically used as a queue.
            vector<shared_ptr<depNode>> fringe;
            fringe.reserve(this->systemOrder.size());
            fringe.push_back(root);
            visited.push_back(root);
            for(auto fringeHead = fringe.begin(); fringeHead != fringe.end(); ++fringeHead) {
                auto node = *fringeHead;
                visited.push_back(node);
                bool parentsAdded = false;
                for(auto n : node->parents) {
                    if(std::find(visited.begin(), visited.end(), n.lock()) == visited.end()
                            && std::find(fringe.begin(), fringe.end(), n.lock()) == fringe.end()) {
                        if(auto ptr = n.lock()) {
                            fringe.insert(fringeHead, ptr);
//                            fringe.push_back(ptr);
                        } else {
                            std::cerr << "ERROR: Ptr invalid!" << std::endl;
                        }
                        parentsAdded = true;
                    }
                }
                if(parentsAdded) {
                    fringe.push_back(node);
                } else {
                    this->systemOrder[listIndex].push_back(node->system);
                    for(auto n : node->children) {
                        if(std::find(visited.begin(), visited.end(), n) == visited.end()
                            && std::find(fringe.begin(), fringe.end(), n) == fringe.end()) {
                            fringe.push_back(n);
                        }
                    }
                }
            }
        }
        
        bool SystemManager::__isGraphCircular(const shared_ptr<depNode>& node, vector<shared_ptr<depNode>> visited) const {
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
        
        bool SystemManager::isGraphCircular(const vector<shared_ptr<depNode>>& roots) const {
            if(roots.size() == 0) {
                // If it does not have a node without any dependencies it is a circle.
                return true;
            }
            for(auto& root : roots) {
                vector<shared_ptr<depNode>> visited;
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

