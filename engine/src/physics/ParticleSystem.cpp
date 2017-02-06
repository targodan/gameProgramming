/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystem.cpp
 * Author: markus
 * 
 * Created on 25. Januar 2017, 12:17
 */

#include "ParticleSystem.h"

#include "../util/vector.h"
namespace engine {
    namespace physics {
        using engine::util::vector;
        
        ParticleSystem::ParticleSystem(float mass, float dampening, shared_ptr<InstanceMesh> mesh, VectorXf force) :
            first(true),
            mass(mass),
            dampening(dampening),
            numParticles(mesh->getInstancePositions().size()/3),
            mesh(mesh),
            positions(Eigen::Map<VectorXf>(mesh->getInstancePositions().data(), 3*this->numParticles)),
            initialForce(force),
            lastVelocities(VectorXf::Zero(3*this->numParticles))
        {
            LOG(INFO) << "initialPos:" << this->positions;
            LOG(INFO) << "initialForce:" << this->initialForce;
            LOG(INFO) << "initialVelo:" << this->lastVelocities;
        }

        ParticleSystem::ParticleSystem(const ParticleSystem& orig) :
            first(orig.first),
            mass(orig.mass),
            dampening(orig.dampening),
            numParticles(orig.numParticles),
            mesh(orig.mesh),
            positions(orig.positions),
            initialForce(orig.initialForce) ,
            lastVelocities(orig.lastVelocities)
        {
        }

        ParticleSystem::~ParticleSystem() {
        }
        
        void ParticleSystem::calculateVelocities(float deltaT, VectorXf force) {
            this->calculateStep(deltaT);
            if(this->first){
                force += this->initialForce;
                this->first = false;
            }
            VectorXf diff = stepMatrixSolver.solve(deltaT * (this->calculateDampeningForce() + force));
            LOG(INFO) << "diff" << diff;
            this->lastVelocities = this->lastVelocities + diff;
        }
        
        SparseMatrix<float> ParticleSystem::calculateMassMatrix() {
            SparseMatrix<float> massMat(3*this->numParticles, 3*this->numParticles);
            massMat.setIdentity();
            return this->mass * massMat;
        }
        
        VectorXf ParticleSystem::calculateDampeningForce() {
            
            return -(this->dampening) * this->lastVelocities;
            
        }
        
        SparseMatrix<float> ParticleSystem::calculateDampeningDerivative() {
            SparseMatrix<float> force(3*this->numParticles, 3*this->numParticles);
            force.setIdentity();
            return -(this->dampening) * force;
        }
        
        void ParticleSystem::calculateStep(float deltaT) {
            SparseMatrix<float> step(3*this->numParticles, 3*this->numParticles);
            step = this->calculateMassMatrix() - deltaT*this->calculateDampeningDerivative();
            stepMatrixSolver.analyzePattern(step);
            stepMatrixSolver.factorize(step);
        }
        
        void ParticleSystem::step(float deltaT, VectorXf force) {
            this->calculateVelocities(deltaT, force);
            this->positions = this->positions + deltaT * this->lastVelocities;
            vector<float> pos(this->positions.data(), this->positions.data() + this->positions.rows());
            this->mesh->setInstancePositions(pos);
            this->mesh->setInstancePositionsChanged(true);
            LOG(INFO) << "Pos" << this->positions;
        }
        
        void ParticleSystem::step(float deltaT) {
            VectorXf force = VectorXf::Zero(3*this->numParticles);
            this->step(deltaT, force);
        }
    }
}