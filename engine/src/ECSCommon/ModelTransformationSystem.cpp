#include "ModelTransformationSystem.h"
#include "PlacementComponent.h"
#include "VisualComponent.h"

#include "../ECS/SystemRegisterer.h"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/string_cast.hpp"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_SYSTEM(ModelTransformationSystem);
        
        systemId_t ModelTransformationSystem::systemId = 0;
        
        using glm::mat4;
        void ModelTransformationSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto it = em.begin({VisualComponent::getComponentTypeId(), PlacementComponent::getComponentTypeId()}); it != em.end(); ++it) {
                auto& placement = it[1]->to<PlacementComponent>();
                auto& visual = it[0]->to<VisualComponent>();
                
                mat4 modelMatrix = glm::translate(placement.getPosition()); // Ignore rotation for now
                visual.setShaderUniform("modelMatrix", modelMatrix);
            }
        }
        
        Array<systemId_t> ModelTransformationSystem::getOptionalDependencies() const {
            return {PlacementSystem::systemTypeId()};
        }
        
        systemId_t ModelTransformationSystem::getSystemTypeId() const {
            return ModelTransformationSystem::systemId;
        }
        
        systemId_t ModelTransformationSystem::systemTypeId() {
            return ModelTransformationSystem::systemId;
        }
        
        void ModelTransformationSystem::setSystemTypeId(systemId_t id) {
            ModelTransformationSystem::systemId = id;
        }
    }
}