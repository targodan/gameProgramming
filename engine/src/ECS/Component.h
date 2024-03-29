#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <stdlib.h>
#include <memory>

#include "EntityId.h"
#include "../CastException.h"

namespace engine {
    namespace ECS {
        typedef std::size_t componentId_t;
        
        class Entity;
        using std::shared_ptr;
        
        class Component {
        protected:
            entityId_t entityId;

            friend class Entity;
            friend class EntityManager;
            void setEntityId(const entityId_t& id) {
                this->entityId = id;
            }
        public:
            Component() {}
            Component(const Component& orig) = delete;
            virtual ~Component() {}

            virtual entityId_t getEntityId() const {
                return this->entityId;
            }
            
            template<class T>
            T& to() {
#ifdef DEBUG
                T* ret = dynamic_cast<T*>(this);
                if(ret == nullptr) {
                    throw CastException("Type %s is not castable to %s!", typeid(this).name(), typeid(T).name());
                }
                return *ret;
#else
                return static_cast<T&>(*this);
#endif
            }
            
            template<class T>
            const T& to() const {
#ifdef DEBUG
                const T* ret = dynamic_cast<const T*>(this);
                if(ret == nullptr) {
                    throw CastException("Type %s is not castable to %s!", typeid(this).name(), typeid(T).name());
                }
                return *ret;
#else
                return static_cast<const T&>(*this);
#endif
            }
            
            virtual componentId_t getComponentId() const = 0;
            virtual std::string getComponentName() const = 0;
            virtual std::string toString() const = 0;
        };
    }
}

#endif /* COMPONENT_H */

