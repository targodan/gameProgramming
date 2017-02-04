#include "VisualObject.h"
#include "ModelLoader.h"

namespace engine {
    namespace renderer {
        using namespace Assimp;
        using util::getAbsoluteFromRelativePath;
        using util::vector;
            
        VisualObject::VisualObject() : initialized(false) {
            
        }
        VisualObject::VisualObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) : initialized(false), mesh(mesh), material(material) {           
            this->init();
        }
        VisualObject::VisualObject(string pathToModel, string pathToVertexShader, string pathToFragmentShader) : initialized(false) {
            ModelLoader loader = {pathToModel, pathToVertexShader, pathToFragmentShader};
            this->mesh = loader.mesh;
            this->material = loader.material;
            
            this->init();
        }

        VisualObject::VisualObject(const VisualObject& orig) : initialized(orig.initialized), mesh(std::shared_ptr<Mesh>(orig.mesh->clone())), material(mesh->getMaterial()) {
            //this->mesh = std::make_shared<Mesh>(*(orig.mesh));
            //this->material = std::make_shared<Material>(*(orig.material));
            
            this->init();
        }
        VisualObject::VisualObject(VisualObject&& orig) : initialized(std::move(orig.initialized)), mesh(std::move(orig.mesh)), material(std::move(orig.material)) {}
        
        VisualObject& VisualObject::operator=(const VisualObject& right) {
            this->mesh = std::make_shared<Mesh>(*right.mesh);
            this->material = this->mesh->getMaterial();
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
            this->mesh->setMaterial(this->material);
            this->initialized = true;
        }

        void VisualObject::loadObject() {
            if(!this->initialized) {
                throw WTFException("Could not load object: object not initialized!");
            }
            
            this->mesh->loadMesh();
            if(this->material->getTextures().size() > 0) {
                this->material->loadTextures();
            }
        }
        void VisualObject::render() {
            if(!this->initialized) {
                throw WTFException("Could not load object: object not initialized!");
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
    }
}