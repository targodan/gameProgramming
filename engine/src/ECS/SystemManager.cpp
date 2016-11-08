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
            this->systemsPreAnalysis.clear();
            this->systems.clear();
        }
        
        void SystemManager::computeRunOrder() {
            if(!this->checkDependencySatisfaction()) {
                throw DependencyException("Some enabled Systems have unsatisfied dependencies.");
            }
            vector<shared_ptr<depNode>> roots = this->buildDependencyGraph();
            if(this->isGraphCircular(roots)) {
                throw DependencyException("Your System dependencies are circular. This is not allowed!");
            }
            size_t parent = 0;
            vector<shared_ptr<depNode>> visited;
            for(auto& root : roots) {
                this->systems.push_back({});
                parent = this->linearizeDependencyGraph(root, visited, this->systems.size()-1, parent);
            }
            this->dbg_printSystems();
            for(size_t i = 0; i < this->systems.size(); ++i) {
                if(this->systems[i].size() == 0) {
                    continue;
                }
                for(size_t j = 0; j < this->systems.size(); ++j) {
                    if(i == j || this->systems[j].size() == 0) {
                        continue;
                    }
                    try {
                        this->systems[i] = this->mergeDependencySublists(
                                this->systems[i],
                                this->systems[j]);
                        this->systems[j].clear();
                    } catch(WTFException) {
                        continue;
                    }
                }
            }
            // Remove empty lists.
            auto newEnd = std::remove_if(this->systems.begin(), this->systems.end(),
                    [](auto list) {return list.size() == 0;});
            this->systems.erase(newEnd, this->systems.end());
        }
        
        vector<shared_ptr<SystemManager::depNode>> SystemManager::buildDependencyGraph() const {
            vector<shared_ptr<SystemManager::depNode>> roots;
            vector<unique_ptr<vector<systemId_t>>> optDeps;
            for(auto& sys : this->systemsPreAnalysis) {
                auto dArr = sys->getOptionalDependencies();
                auto deps = std::make_unique<vector<systemId_t>>();
                deps->reserve(dArr.size());
                for(auto d : dArr) {
                    for(auto& s : this->systemsPreAnalysis) {
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
            
            vector<shared_ptr<SystemManager::depNode>> fringe;
            // Create all nodes and find roots.
            for(size_t i = 0; i < this->systemsPreAnalysis.size(); ++i) {
                auto& sys = this->systemsPreAnalysis[i];
                auto node = std::make_shared<SystemManager::depNode>(sys);
                fringe.push_back(node);
                if(sys->getDependencies().size() == 0 && optDeps[i]->size() == 0) {
                    roots.push_back(node);
                }
            }
            
            // Create all links.
            for(size_t i = 0; i < fringe.size(); ++i) {
                auto& requiredBy = fringe[i];
                for(auto& dep : requiredBy->system->getDependencies()) {
                    for(auto& dependency : fringe) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                        }
                    }
                }
                for(auto& dep : *optDeps[i]) {
                    for(auto& dependency : fringe) {
                        if(dependency->system->getSystemTypeId() == dep) {
                            dependency->children.push_back(requiredBy);
                        }
                    }
                }
            }
            
            return roots;
        }
        
        vector<shared_ptr<System>> SystemManager::mergeDependencySublists(
                const vector<shared_ptr<System>>& primary,
                const vector<shared_ptr<System>>& secondary) const {
            vector<shared_ptr<System>> ret;
            auto splitpoint = std::find(primary.begin(), primary.end(), secondary[0]);
            auto mergepoint = std::find(primary.begin(), primary.end(), secondary[secondary.size()-1]);
            if(splitpoint == primary.end() && mergepoint == primary.end()) {
                throw WTFException("Could not find merge or split point while merging.");
            }
            if(splitpoint == mergepoint) {
                throw WTFException("Looks like there are two links from the same origin-node to the same target-node. This should never happen.");
            }
            ret.reserve(primary.size() + secondary.size() - 2);
            if(splitpoint != primary.end() && mergepoint != primary.end()) {
                // Split and merge point
                for(auto it = primary.begin(); it != splitpoint; ++it) {
                    // Copy primary until split point
                    ret.push_back(*it);
                }
                for(auto it = secondary.begin(); it != secondary.end()-1; ++it) {
                    // Now copy secondary.
                    // But watch out not to copy the merge point twice.
                    ret.push_back(*it);
                }
                for(auto it = splitpoint+1; it != primary.end(); ++it) {
                    // Now copy the rest of primary.
                    ret.push_back(*it);
                }
            } else if(mergepoint != primary.end()) {
                // only a merge point
                for(auto it = primary.begin(); it != mergepoint; ++it) {
                    // Copy primary until merge point
                    ret.push_back(*it);
                }
                for(auto it = secondary.begin(); it != secondary.end()-1; ++it) {
                    // Now copy secondary.
                    // But watch out not to copy the merge point twice.
                    ret.push_back(*it);
                }
                for(auto it = mergepoint; it != primary.end(); ++it) {
                    // Now copy the rest of primary.
                    ret.push_back(*it);
                }
            } else {
                // only a split point
                for(auto it = primary.begin(); it != splitpoint; ++it) {
                    // Copy primary until split point
                    ret.push_back(*it);
                }
                for(auto it = secondary.begin(); it != secondary.end(); ++it) {
                    // Now copy secondary.
                    ret.push_back(*it);
                }
                for(auto it = splitpoint+1; it != primary.end(); ++it) {
                    // Now copy the rest of primary.
                    ret.push_back(*it);
                }
            }
            return ret;
        }
        
        void SystemManager::dbg_printSystems() const {
            std::cout << this->systems << std::endl;
        }
        
        size_t SystemManager::linearizeDependencyGraph(const shared_ptr<depNode>& node,
                vector<shared_ptr<depNode>>& visited,
                size_t listIndex, size_t parentIndex) {
            this->systems[listIndex].push_back(node->system);
            if(std::find(visited.begin(), visited.end(), node) != visited.end() || node->children.size() == 0) {
                visited.push_back(node);
                if(listIndex == parentIndex) {
                    return parentIndex;
                }
                for(int i = 0; i < this->systems.size(); ++i) {
                    if(i == listIndex || this->systems[i].size() == 0) {
                        continue;
                    }
                    try {
                        this->systems[i] = this->mergeDependencySublists(
                                this->systems[i],
                                this->systems[listIndex]);
                        this->systems[listIndex].clear();
                        return i;
                    } catch(WTFException) {
                    }
                }
                        return listIndex;
            } else {
                visited.push_back(node);
                bool firstChild = true;
                for(auto& child : node->children) {
                    if(firstChild) {
                        firstChild = false;
                        listIndex = this->linearizeDependencyGraph(child, visited, listIndex, parentIndex);
                    } else {
                        this->systems.push_back({node->system});
                        size_t newListIndex = this->systems.size()-1;
                        listIndex = this->linearizeDependencyGraph(child, visited, newListIndex, listIndex);
                    }
                }
                return listIndex;
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
            for(auto& sys : this->systemsPreAnalysis) {
                for(auto depId : sys->getDependencies()) {
                    bool satisfied = false;
                    for(auto& sysDep : this->systemsPreAnalysis) {
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

