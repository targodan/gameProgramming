#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <type_traits>
#include <vector>
#include <memory>
#include <thread>
#include <future>

#include "System.h"
#include "EntityManager.h"
#include "../util/Array.h"
#include "../util/BlockingQueue.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using engine::util::Array;
using engine::util::BlockingQueue;

namespace engine {
    namespace ECS {
        class SystemManager {
        protected:
            struct SystemNode {
                SystemNode(shared_ptr<System>& sys) : system(sys) {}
                vector<shared_ptr<SystemNode>> children;
                vector<weak_ptr<SystemNode>> parents;
                shared_ptr<System> system;
            };
            
            struct Task {
                Task(shared_ptr<System> system, unique_ptr<std::promise<void>> promise)
                        : system(system), promise(std::move(promise)) {}
                shared_ptr<System> system;
                unique_ptr<std::promise<void>> promise;
                virtual bool stop() const {
                    return false;
                }
            };
            
            struct StopTask : public Task {
                StopTask() : Task(nullptr, nullptr) {}
                bool stop() const override {
                    return true;
                }
            };
            
            EntityManager& em;
            
            vector<shared_ptr<System>> enabledSystems;
            vector<shared_ptr<SystemNode>> dependencyTree;
            
            BlockingQueue<unique_ptr<Task>> queue;
            Array<std::thread> threads;
            
            bool checkDependencySatisfaction() const;
            vector<shared_ptr<SystemNode>> buildDependencyGraph() const;
            bool __isGraphCircular(const shared_ptr<SystemNode>& root, vector<shared_ptr<SystemNode>> visited) const;
            bool isGraphCircular(const vector<shared_ptr<SystemNode>>& roots) const;
            size_t numThreads() const;
            
        public:
            SystemManager(EntityManager& em);
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
                this->enabledSystems.push_back(sys);
                return sys;
            }
            
            void setup();
            void stop();
            
            void run();
        };
    }
}

#endif /* SYSTEMMANAGER_H */

