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
#include <vector>
#include <memory>
#include "../renderer/Mesh.h"
#include <eigen3/Eigen/Eigen>

namespace engine {
    namespace physics {
        using namespace Eigen;
        using std::vector;
        using engine::renderer::Mesh;
        using std::shared_ptr;
        class ParticleSystem {
            using SparseSolver = SparseLU<SparseMatrix<float, ColMajor>, COLAMDOrdering<SparseMatrix<float, ColMajor>::StorageIndex>>;
        public:
            ParticleSystem(float mass, float dampening, vector<Mesh> parts, VectorXf positions, VectorXf Force);
            ParticleSystem(const ParticleSystem& orig);
            virtual ~ParticleSystem();
            
            void step(float deltaT, VectorXf force);
            void step(float deltaT);
        protected:
                    bool first;
                    float mass; // in kg
                    float dampening;
                    vector<Mesh> parts; //just for debug*/
                    
                    VectorXf positions;
                    VectorXf initialForce;
                    
                    VectorXf lastVelocities;
                    
                    SparseSolver stepMatrixSolver;
                    VectorXf calculateVelocities(float deltaT, VectorXf force);
                    SparseMatrix<float> calculateMassMatrix();
                    VectorXf calculateDampeningForce();
                    SparseMatrix<float> calculateDampeningDerivative();
                    void calculateStep(float deltaT);
                    
                    
                    
                    

        };
    }
}

#endif /* PARTICLESYSTEM_H */

