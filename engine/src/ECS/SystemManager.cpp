#include "SystemManager.h"

#include "DependencyException.h"

#include <iostream>
#include <algorithm>

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
            for(size_t i = 0; i < this->systemsPreAnalysis.size(); ++i) {
                
            }
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
            auto mergepoint = std::find(splitpoint, primary.end(), secondary[secondary.size()-1]);
            if(splitpoint == primary.end() || mergepoint == primary.end()) {
                throw WTFException("Could not find merge or split point while merging.");
            }
            if(splitpoint == mergepoint) {
                throw WTFException("Looks like there are two links from the same origin-node to the same target-node. This should never happen.");
            }
            if(secondary.size() == 2) {
                // Nothing to add.
                return primary;
            }
            ret.reserve(primary.size() + secondary.size() - 2);
            for(auto it = primary.begin(); it != splitpoint; ++it) {
                // Copy primary until splitpoint
                ret.push_back(*it);
            }
            for(auto it = secondary.begin(); it != secondary.end()-1; ++it) {
                // Now copy secondary.
                // But watch out not to copy the mergepoint twice.
                ret.push_back(*it);
            }
            for(auto it = splitpoint+1; it != primary.end(); ++it) {
                // Now copy the rest of primary.
                ret.push_back(*it);
            }
            return ret;
        }
        
        void SystemManager::linearizeDependencyGraph(const shared_ptr<depNode>& node, vector<shared_ptr<depNode>>& visited) {
            
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

