/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystem.h
 * Author: markus
 *
 * Created on 25. Januar 2017, 12:17
 */

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "constants.h"
#include <memory>
#include "../renderer/InstanceMesh.h"
#include <eigen3/Eigen/Eigen>
#include "ObjectProperties.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using engine::renderer::InstanceMesh;
        using std::shared_ptr;
        class ParticleSystem {
            using SparseSolver = SparseLU<SparseMatrix<float, ColMajor>, COLAMDOrdering<SparseMatrix<float, ColMajor>::StorageIndex>>;
            
        public:
            ParticleSystem(float mass, float surfaceArea, float dampening, shared_ptr<InstanceMesh> mesh, VectorXf Force);
            ParticleSystem(const ParticleSystem& orig);
            virtual ~ParticleSystem();
            
            void step(float deltaT, const VectorXf& force);
            void step(float deltaT);
            int getNumParticles();
            bool isEnabled() {
                return this->enabled;
            }
            void enable(bool set=true) {
                this->enabled = set;
            }
            
            ObjectProperties getObjectProperties() const {
                const auto numVert = this->mesh->getInstancePositions().size() / 3;
                vector<size_t> indices;
                indices.reserve(numVert);
                VectorXf surfaceAreas(numVert);
                VectorXf masses(numVert);
                for(size_t i = 0; i < numVert; ++i) {
                    indices.push_back(i);
                    surfaceAreas[i] = this->surfaceArea;
                    masses[i] = this->mass;
                }
                Eigen::Map<VectorXf> allVertices(mesh->getInstancePositions().data(), this->mesh->getInstancePositions().size());
                return ObjectProperties(
                        allVertices,
                        indices,
                        surfaceAreas,
                        masses
                    );
            }
            
            int getExpectedForceVectorSize() const {
                return this->mesh->getInstancePositions().size();
            }
            
        protected:
            bool enabled;
            bool first;
            float mass; // in kg
            float surfaceArea;
            float dampening;
            int numParticles;
            shared_ptr<InstanceMesh> mesh;

            VectorXf positions;
            VectorXf initialForce;

            VectorXf lastVelocities;

            SparseSolver stepMatrixSolver;
            void calculateVelocities(float deltaT, VectorXf force);
            SparseMatrix<float> calculateMassMatrix();
            VectorXf calculateDampeningForce();
            SparseMatrix<float> calculateDampeningDerivative();
            void calculateStep(float deltaT);
        };
    }
}

#endif /* PARTICLESYSTEM_H */

