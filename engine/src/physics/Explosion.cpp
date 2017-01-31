#include "Explosion.h"

#include <math.h>
#include <valarray>

#include <easylogging++.h>

namespace engine {
    namespace physics {
        Explosion& Explosion::operator=(const Explosion& orig) {
            this->center = orig.center;
            this->expansionSpeed = orig.expansionSpeed;
            this->lastSecondsFromExplosion = orig.lastSecondsFromExplosion;
            this->secondsFromExplosion = orig.secondsFromExplosion;
            this->secondsSinceStart = orig.secondsSinceStart;
            this->tntEquivalence = orig.tntEquivalence;
            return *this;
        }
        Explosion& Explosion::operator=(Explosion&& orig) {
            std::swap(this->center, orig.center);
            std::swap(this->expansionSpeed, orig.expansionSpeed);
            std::swap(this->lastSecondsFromExplosion, orig.lastSecondsFromExplosion);
            std::swap(this->secondsFromExplosion, orig.secondsFromExplosion);
            std::swap(this->secondsSinceStart, orig.secondsSinceStart);
            std::swap(this->tntEquivalence, orig.tntEquivalence);
            return *this;
        }
            
        float Explosion::calculatePressureAtDistance(float d) const {
            // Source: https://de.wikipedia.org/wiki/Sprengkraft (solved for P by Luca)
            return std::exp(
                        6.4221
                        - std::sqrt(
                            6.4221 * 6.4221
                            - 25.1256 * (
                                0.9267
                                - std::log(d / std::cbrtf(this->tntEquivalence))
                            )
                        )
                    ) * 1e-5; // bar -> Pa
        }
        
        float Explosion::calculateExpansionRadius(float secondsFromExplosion) const {
            return this->expansionSpeed * secondsFromExplosion;
        }
        
        void Explosion::setTime(float secondsFromExplosion) {
            this->lastSecondsFromExplosion = this->secondsFromExplosion;
            this->secondsFromExplosion = secondsFromExplosion;
        }
        
        MatrixXf Explosion::calculateDistancesVectorsFromCenter(const ObjectProperties& object) const {
            size_t numVertices = object.surfaceVertexIndices.size();
            MatrixXf centers(3, numVertices);
            for(size_t i = 0; i < numVertices; ++i) {
                centers.col(i) = this->center;
            }
            
            VectorXf surface = object.getSurfaceVertices();
            const Map<MatrixXf> surfaceVerticesInColumns(surface.data(), 3, numVertices);
            
            return surfaceVerticesInColumns - centers;
        }
        
        VectorXf Explosion::calculateSqDistancesFromCenter(const MatrixXf& distanceVectors) const {
            return distanceVectors.colwise().squaredNorm().transpose();
        }
        
        VectorXf Explosion::mapAffectedForcesToSurface(const MatrixXf& sqDistances, const MatrixXf& affectedForceVectors, const ObjectProperties& object) const {
            VectorXf forceVectors(object.surfaceVertexIndices.size() * 3, 1);
            int indexOfAffected = 0;
            for(size_t i = 0; i < object.surfaceVertexIndices.size() * 3; i += 3) {
                int indexVector = i / 3;
                if(indexOfAffected >= affectedForceVectors.cols() || sqDistances(indexVector) == 0) {
                    forceVectors(i+0) = 0;
                    forceVectors(i+1) = 0;
                    forceVectors(i+2) = 0;
                } else {
                    forceVectors(i+0) = affectedForceVectors(0, indexOfAffected);
                    forceVectors(i+1) = affectedForceVectors(1, indexOfAffected);
                    forceVectors(i+2) = affectedForceVectors(2, indexOfAffected);
                    ++indexOfAffected;
                }
            }
            return forceVectors;
        }
        
        MatrixXf Explosion::calculateAffectedParameters(const ObjectProperties& object, VectorXf& sqDistances, const MatrixXf& distanceVectors) const {
            MatrixXf affectedForceVectors(3, distanceVectors.cols());
            VectorXf affectedDistances(distanceVectors.cols());
            VectorXf affectedSurfaceAreas(distanceVectors.cols());
            
            float lastExpansionRadiusSq = this->calculateExpansionRadius(this->lastSecondsFromExplosion);
            lastExpansionRadiusSq *= lastExpansionRadiusSq;
            float expansionRadiusSq = this->calculateExpansionRadius(this->secondsFromExplosion);
            expansionRadiusSq *= expansionRadiusSq;
            
            // set all outside to zero
            int numAffectedVectors = 0;
            for(int i = 0; i < sqDistances.rows(); ++i) {
                // TODO: Maybe this is possible with a Map.
                if(sqDistances(i) <= lastExpansionRadiusSq || expansionRadiusSq < sqDistances(i)) {
                    // Since a distance of 0 would result in an infinite pressure anyway 0 is a safe number here
                    sqDistances(i) = 0;
                } else {
                    affectedDistances(numAffectedVectors) = sqDistances(i);
                    affectedSurfaceAreas(numAffectedVectors) = object.surfaceAreaPerVertex(i);
                    affectedForceVectors.col(numAffectedVectors) = distanceVectors.col(i);
                    ++numAffectedVectors;
                }
            }
            if(numAffectedVectors == 0) {
                return MatrixXf(0, 0);
            }
            
            
            MatrixXf reducedAffectedForceVectors = affectedForceVectors.block(0, 0, 3, numAffectedVectors);
            VectorXf reducedAffectedDistances = affectedDistances.block(0, 0, numAffectedVectors, 1).cwiseSqrt();
            
            for(int i = 0; i < numAffectedVectors; ++i) {
                // normalize and multiply by force
                reducedAffectedForceVectors.col(i) *= (
                            this->calculatePressureAtDistance(reducedAffectedDistances(i)) * affectedSurfaceAreas(i)
                        ) / reducedAffectedDistances(i);
            }
            
            return reducedAffectedForceVectors;
        }

        VectorXf Explosion::getForceOnVertices(const ObjectProperties& object) {
            if(this->secondsSinceStart < 0) {
                return VectorXf(0);
            }
            
            this->setTime(this->secondsSinceStart);
            
            MatrixXf distanceVectors = this->calculateDistancesVectorsFromCenter(object);
            VectorXf sqDistances = this->calculateSqDistancesFromCenter(distanceVectors);
            
            MatrixXf affectedForceVectors = this->calculateAffectedParameters(object, sqDistances, distanceVectors);
            if(affectedForceVectors.rows() == 0) {
                return VectorXf(0);
            }
            
            auto forces = object.mapSurfaceForcesToAllVertices(this->mapAffectedForcesToSurface(sqDistances, affectedForceVectors, object));
            LOG(INFO) << "F" << std::endl << forces;
            return forces;
        }
    }
}
