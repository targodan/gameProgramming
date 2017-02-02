#ifndef ADDENTITYMESSAGE_H
#define ADDENTITYMESSAGE_H

#include <memory>

#include "../util/Array.h"

#include "Message.h"
#include "MessageHandler.h"
#include "Entity.h"
#include "Component.h"

#define MESSAGE_NAME_ADD_ENTITY_MESSAGE "ADD_ENTITY_MESSAGE"

namespace engine {
    namespace ECS {
        using engine::util::Array;
        
        class AddEntityMessage : public Message {
        protected:
            std::string name;
            Entity* newEntity;
            Array<std::shared_ptr<Component>> components;
            
            static messageId_t messageId;
            static bool messageRegistered;
            
        public:
            AddEntityMessage(const std::string& name, const Array<std::shared_ptr<Component>>& components, Entity* out_newEntity = nullptr)
                    : Message(AddEntityMessage::messageId), name(name), newEntity(out_newEntity), components(components) {}
            AddEntityMessage(const AddEntityMessage& orig)
                    : Message(orig), name(orig.name), newEntity(orig.newEntity), components(orig.components) {}
            AddEntityMessage(AddEntityMessage&& orig)
                    : Message(std::move(orig)), name(std::move(orig.name)), newEntity(std::move(orig.newEntity)), components(std::move(orig.components)) {}
                    
            const Array<std::shared_ptr<Component>>& getComponents() const {
                return components;
            }

           const std::string& getName() const {
                return name;
            }

            Entity* getNewEntityPtr() const {
                return newEntity;
            }
            
            static void setMessageId(messageId_t id);
            static messageId_t getMessageId();
            
            static void registerMessageName(MessageHandler& handler);
        };
    }
}

#endif /* ADDENTITYMESSAGE_H */

