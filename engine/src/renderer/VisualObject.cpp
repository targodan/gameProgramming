#include "VisualObject.h"
#include "ModelLoader.h"
#include <iostream>
#include <easylogging++.h>

namespace engine {
    namespace renderer {
        using namespace Assimp;
        using util::getAbsoluteFromRelativePath;
        using util::vector;
            
        VisualObject::VisualObject() {
            
        }
        VisualObject::VisualObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) : mesh(mesh), material(material) {           
            this->init();
        }
        VisualObject::VisualObject(string pathToModel, Mapping mapping, bool lighting) {
            ModelLoader loader = {pathToModel, mapping, lighting};
            this->mesh = loader.mesh;
            this->material = loader.material;
            
            this->init();
        }

        VisualObject::VisualObject(const VisualObject& orig) : mesh(std::shared_ptr<Mesh>(orig.mesh->clone())), material(std::make_shared<Material>(*(orig.material))), loaded(false) {
            this->init();
        }
        VisualObject::VisualObject(VisualObject&& orig) : initialized(std::move(orig.initialized)), mesh(std::move(orig.mesh)), material(std::move(orig.material)) {}
        
        VisualObject& VisualObject::operator=(const VisualObject& right) {
            this->mesh = std::shared_ptr<Mesh>(right.mesh->clone());
            this->material = std::make_shared<Material>(*(right.material));
            this->init();
            
            return *this;
        }
        VisualObject& VisualObject::operator=(VisualObject&& right) {
            this->mesh = std::move(right.mesh);
            this->material = std::move(right.material);
            
            return *this;
        }

        VisualObject::~VisualObject() {

        }
        
        void VisualObject::init() {
            if(this->material->getShader()) {
                this->mesh->setMaterial(this->material);
                this->initialized = true;
            }
        }

        void VisualObject::loadObject() {
            if(!this->initialized) {
                this->init();
                if(!this->initialized) {  
                    throw WTFException("Could not load object: could not initialize object. Maybe the shaders aren't correctly set?");
                }
            }
            
            this->mesh->loadMesh();
            if(this->material->getTextures().size() > 0) {
                this->material->loadTextures();
                LOG(INFO) << "Loaded textures.";
            }
            
            this->loaded = true;
        }
        void VisualObject::render() {
            if(!this->initialized) {
                throw WTFException("Could not load object: could not initialize object. Maybe the shaders aren't correctly set?");
            }
            
            this->mesh->render();
        }

        const Mesh& VisualObject::getMesh() const {
            return *this->mesh;
        }
        const Material& VisualObject::getMaterial() const {
            return *this->material;
        }
        Mesh& VisualObject::getMesh() {
            return *this->mesh;
        }
        Material& VisualObject::getMaterial() {
            return *this->material;
        }
        
        bool VisualObject::isInitialized() const {
            return this->initialized;
        }
        bool VisualObject::isLoaded() const {
            return this->loaded;
        }
    }
}