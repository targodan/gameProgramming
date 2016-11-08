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
                this->linearizeDependencyGraph(root, visited, this->systems.size()-1, parent);
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
                        std::cerr << "MERGIN:" << std::endl
                                << this->systems[i] << std::endl
                                << "with" << std::endl
                                << this->systems[j] << std::endl;
                        this->systems[i] = this->mergeDependencySublists(
                                this->systems[i],
                                this->systems[j]);
                        std::cerr << "RESULT:" << std::endl
                                << this->systems[i] << std::endl
                                << std::endl;
                        this->systems[j].clear();
                    } catch(WTFException) {
                        std::cerr << "FAIL" << std::endl
                                << std::endl;
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
            std::cout << this->systems << std::endl;
        }
        
        size_t SystemManager::linearizeDependencyGraph(const shared_ptr<depNode>& node,
                vector<shared_ptr<depNode>>& visited,
                size_t listIndex, size_t parentIndex) {
            this->systems[listIndex].push_back(node->system);
            if(std::find(visited.begin(), visited.end(), node) != visited.end() || node->children.size() == 0) {
                visited.push_back(node);
                return 0;
            } else {
                visited.push_back(node);
                bool firstChild = true;
                for(auto& child : node->children) {
                    if(firstChild) {
                        firstChild = false;
                        this->linearizeDependencyGraph(child, visited, listIndex, parentIndex);
                    } else {
                        this->systems.push_back({node->system});
                        size_t newListIndex = this->systems.size()-1;
                        this->linearizeDependencyGraph(child, visited, newListIndex, listIndex);
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

