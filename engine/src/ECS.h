#ifndef ECS_H
#define ECS_H

#include "./ECS/Component.h"
#include "./ECS/ComponentRegisterer.h"
#include "./ECS/ComponentRegistry.h"
#include "./ECS/DependencyException.h"
#include "./ECS/Entity.h"
#include "./ECS/EntityId.h"
#include "./ECS/EntityManager.h"
#include "./ECS/Message.h"
#include "./ECS/MessageHandler.h"
#include "./ECS/MessageReceiver.h"
#include "./ECS/SerializableComponent.h"
#include "./ECS/System.h"
#include "./ECS/SystemManager.h"
#include "./ECS/SystemRegisterer.h"
#include "./ECS/SystemRegistry.h"
#include "./ECS/SystemType.h"
#include "./ECS/UnknownComponentException.h"
#include "./ECS/macros.h"
#include "./ECS/messageId.h"

#include "./ECS/pb/EntityManager.pb.h"

#endif /* ECS_H */

