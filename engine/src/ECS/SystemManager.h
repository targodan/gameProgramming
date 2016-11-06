#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <type_traits>
#include <vector>
#include <memory>

#include "System.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

namespace engine {
    namespace ECS {
        class SystemManager {
        protected:
            vector<shared_ptr<System>> systemsPreAnalysis;
            vector<unique_ptr<vector<shared_ptr<System>>>> systems;
            
            struct depNode {
                depNode(shared_ptr<System> sys) : system(sys) {}
                vector<shared_ptr<depNode>> children;
                shared_ptr<System> system;
            };
            
            bool checkDependencySatisfaction() const;
            vector<shared_ptr<depNode>> buildDependencyGraph() const;
            bool __isGraphCircular(const shared_ptr<depNode>& root, vector<shared_ptr<depNode>> visited) const;
            bool isGraphCircular(const vector<shared_ptr<depNode>>& roots) const;
            
        public:
            SystemManager();
            SystemManager(const SystemManager& orig) = delete;
            ~SystemManager();

            template<class SystemT>
            shared_ptr<SystemT> enableSystem() {
#ifdef DEBUG
                if(!std::is_base_of<System, SystemT>::value) {
                    throw WTFException("Only subclasses of engine::ECS::System can be enabled by the SystemManager.");
                }
                if(!std::is_constructible<SystemT>::value) {
                    throw WTFException("The system needs to have a constructor which does not take any arguments.");
                }
#endif
                auto sys = std::make_shared<SystemT>();
                this->systemsPreAnalysis.push_back(sys);
                return sys;
            }
            
            void computeRunOrder();
        };
    }
}

#endif /* SYSTEMMANAGER_H */

