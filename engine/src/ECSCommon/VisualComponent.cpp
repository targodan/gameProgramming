#include "VisualComponent.h"
#include "../ECS/ComponentRegisterer.h"

#include <sstream>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(VisualComponent);
        
        componentId_t VisualComponent::typeId = 0;
        
        VisualComponent::VisualComponent(const Mesh& mesh, const Material& material) 
            : mesh(mesh), material(material) {
        }

        VisualComponent::~VisualComponent() {
        }
        
        void VisualComponent::setMesh(const Mesh& mesh) {
            this->mesh = mesh;
        }
        const Mesh& VisualComponent::getMesh() const {
            return this->mesh;
        }
        Mesh& VisualComponent::getMesh() {
            return this->mesh;
        }

        void VisualComponent::setMaterial(const Material& mat) {
            this->material = mat;
        }
        const Material& VisualComponent::getMaterial() const {
            return this->material;
        }
        Material& VisualComponent::getMaterial() {
            return this->material;
        }
        
        componentId_t VisualComponent::getComponentId() const {
            return VisualComponent::typeId;
        }
        std::string VisualComponent::getComponentName() const {
            return "Visual";
        }
        std::string VisualComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "VisualComponent" << "}";
            return ss.str();
        }
    }
}