#include "InstanceMesh.h"

namespace engine {
    namespace renderer {
        InstanceMesh::InstanceMesh(const vector<Vertex>& vertices, const vector<float>& instancePositions, DataUsagePattern usage)
            : Mesh(vertices), instancePositions(instancePositions), usage(usage) {
#ifdef DEBUG
            if(instancePositions.empty()) {
                throw WTFException("Could not initialize InstanceMesh: No instance positions given!");
            }
#endif
            
            this->createPositionVBO(this->instancePositions, usage);
        }
        
        InstanceMesh::InstanceMesh(const InstanceMesh& orig) 
            : Mesh() {
            this->material = orig.material;
            this->vertices = orig.vertices;
            this->indices = orig.indices;
            this->vao = std::make_unique<VertexArray>();
            this->usage = orig.usage;
            this->instancePositions = orig.instancePositions;
            
            
            if(!this->indices.empty()) {
                this->createEBO(this->indices, this->usage);
            }
            this->createVBO(this->vertices, this->usage);
            this->createPositionVBO(this->instancePositions, this->usage);
            
            if(orig.loaded) {
                this->loadMesh();
            }
        }
        InstanceMesh::InstanceMesh(InstanceMesh&& orig)
            : Mesh(std::move(orig)), instancePositions(std::move(orig.instancePositions)), usage(std::move(orig.usage)) {

        }

        InstanceMesh& InstanceMesh::operator=(const InstanceMesh& right) {
            Mesh::operator=(right);
            this->instancePositions = right.instancePositions;
            this->usage = right.usage;

            this->createPositionVBO(this->instancePositions, this->usage);

            return *this;
        }
        InstanceMesh& InstanceMesh::operator=(InstanceMesh&& right) {
            Mesh::operator=(std::move(right));
            this->instancePositions = std::move(right.instancePositions);
            this->usage = std::move(right.usage);

            return *this;
        }

        InstanceMesh::~InstanceMesh() {

        }

        void InstanceMesh::loadMesh() {
            if(!this->createdBuffer) {
                this->createPositionVBO(this->instancePositions, usage);
                this->createdBuffer = true;
            }
            
            Mesh::loadMesh();
            // this->instancePositionsChanged = true;
        }
        void InstanceMesh::render() {
#ifdef DEBUG
            if(this->material == nullptr) {
                throw WTFException("Cannot render mesh: no material set.");
            }
#endif /*DEBUG*/

            this->vao->bind();

            if(this->verticesChanged || this->instancePositionsChanged) {
                this->vao->reloadData();
                this->verticesChanged = false;
                this->instancePositionsChanged = false;
            }
            if(this->indicesChanged) {
                if(this->indices.size() > 0) {
                    this->vao->reloadIndices();
                }
                this->indicesChanged = false;
            }

            this->vao->unbind();

            this->material->makeActive();

            this->vao->bind();
//            if(this->indices.size() > 0) {
//                this->vao->drawElements();
//            } else {
//                this->vao->drawArrays();
//            }

            this->vao->drawArraysInstanced(this->vertices.size() ,this->instancePositions.size() / 3);
            this->vao->unbind();

            this->material->makeInactive();
        }
//
//        void InstanceMesh::setMaterial(const std::shared_ptr<Material>& material) {
//            Mesh::setMaterial(material);
//
//        }

        const vector<float>& InstanceMesh::getInstancePositions() const {
            return this->instancePositions;
        }
        vector<float>& InstanceMesh::getInstancePositions() {
            return this->instancePositions;
        }
        void InstanceMesh::setInstancePositions(const vector<float>& instanceCentres) {
            this->instancePositions = instanceCentres;
        }

        void InstanceMesh::setInstancePositionsChanged(bool changed) {
            this->instancePositionsChanged = changed;
        }

        void InstanceMesh::createPositionVBO(vector<float>& positions, DataUsagePattern usage) {
            // Get no. of elements
            auto nPositions = positions.size();

#ifdef DEBUG
            if(!nVertices) {
                throw WTFException("Cannot create VBO: no vertices given.");
            }
#endif /*DEBUG*/

            // Create VBO that holds vertex data
            auto vbo = std::make_unique<VertexBuffer>(reinterpret_cast<void*>(positions.data()), sizeof(float) * nPositions, nPositions, usage);
            this->vao->attachVBO(std::move(vbo));
        }

        void InstanceMesh::setVAOAttributes() {
            Mesh::setVAOAttributes();

            auto instancePositionIndex = this->material->getShader()->getAttributeLocation("instancePosition");

            VertexAttribute instancePositionAttrib {instancePositionIndex, 3,
                    DataType::FLOAT, 0, 3*sizeof(float), 0};

            vector<VertexAttribute> attribs;
            attribs.push_back(instancePositionAttrib);

            auto& vbo = this->vao->getVBOs()[1];
            vbo->setAttributes(attribs);

            this->vao->bind();
            vbo->setDivisors(1);
            this->vao->unbind();
        }
    }
}
