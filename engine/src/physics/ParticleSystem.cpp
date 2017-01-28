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
namespace engine {
    namespace physics {
        ParticleSystem::ParticleSystem(float mass, float dampening, /*vector<Mesh> parts,*/ VectorXf positions, VectorXf force) :
            first(true),
            mass(mass),
            dampening(dampening),
           /* parts(parts),*/
            positions(positions),
            initialForce(force),
            lastVelocities(VectorXf::Zero(NUM_OF_PARTICLES))
        {
            
        }

        ParticleSystem::ParticleSystem(const ParticleSystem& orig) :
            first(orig.first),
            mass(orig.mass),
            dampening(orig.dampening),
            /*parts(orig.parts),*/
            positions(orig.positions),
            initialForce(orig.initialForce) ,
            lastVelocities(orig.lastVelocities)
        {
        }

        ParticleSystem::~ParticleSystem() {
        }
        
        VectorXf ParticleSystem::calculateVelocities(float deltaT, VectorXf force) {
            this->calculateStep(deltaT);
            if(this->first){
                force += this->initialForce;
                this->first = false;
            }
            return this->lastVelocities + stepMatrixSolver.solve(deltaT * (this->calculateDampeningForce() + force));
        }
        
        SparseMatrix<float> ParticleSystem::calculateMassMatrix() {
            SparseMatrix<float> massMat(NUM_OF_PARTICLES, NUM_OF_PARTICLES);
            massMat.setIdentity();
            return this->mass * massMat;
        }
        
        VectorXf ParticleSystem::calculateDampeningForce() {
            
            return this->dampening * this->lastVelocities;
            
        }
        
        SparseMatrix<float> ParticleSystem::calculateDampeningDerivative() {
            SparseMatrix<float> force(NUM_OF_PARTICLES, NUM_OF_PARTICLES);
            force.setIdentity();
            return this->dampening * force;
        }
        
        void ParticleSystem::calculateStep(float deltaT) {
            SparseMatrix<float> step(NUM_OF_PARTICLES, NUM_OF_PARTICLES);
            step = this->calculateMassMatrix() - deltaT*this->calculateDampeningDerivative();
            stepMatrixSolver.analyzePattern(step);
            stepMatrixSolver.factorize(step);
        }
        
        void ParticleSystem::step(float deltaT, VectorXf force) {
            this->positions = this->positions + deltaT * this->calculateVelocities(deltaT, force);
            int i = 0;
            /*for(auto it : this->parts) {
                
                it.getVertices()[0].position.x = this->positions[i+0];
                it.getVertices()[0].position.y = this->positions[i+1];
                it.getVertices()[0].position.z = this->positions[i+2]+0.1;
                it.getVertices()[1].position.x = this->positions[i+0]+0.1;
                it.getVertices()[1].position.y = this->positions[i+1];
                it.getVertices()[1].position.z = this->positions[i+2]-0.1;
                it.getVertices()[2].position.x = this->positions[i+0]-0.1;
                it.getVertices()[2].position.y = this->positions[i+1];
                it.getVertices()[2].position.z = this->positions[i+2]-0.1;
                it.getVertices()[3].position.x = this->positions[i+0];
                it.getVertices()[3].position.y = this->positions[i+1]+0.2;
                it.getVertices()[3].position.z = this->positions[i+2];
                
                it->setVerticesChanged(true);
                i++;
            }*/
        }
        
        void ParticleSystem::step(float deltaT) {
            VectorXf force = VectorXf::Zero(NUM_OF_PARTICLES);
            this->step(deltaT, force);
        }
    }
}