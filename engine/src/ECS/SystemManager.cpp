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
        
        void SystemManager::dbg_printSystems() const {
            std::cout << this->systemOrder << std::endl;
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

