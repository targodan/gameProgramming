#include "Explosion.h"

#include <math.h>
#include <valarray>

namespace engine {
    namespace physics {
        float Explosion::calculatePressureAtDistance(float d) const {
            // Source: https://de.wikipedia.org/wiki/Sprengkraft (solved for P by Luca)
            return std::exp(
                        6.4221
                        - std::sqrt(
                            6.4221 * 6.4221
                            - 25.1256 * (
                                0.9267
                                - std::log(d / std::cbrt(this->tntEquivalence))
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
            Index numVertices = object.surfaceVertexIndices.size();
            MatrixXf centers(3, numVertices);
            for(int i = 0; i < numVertices; i += 3) {
                centers.col(i) = this->center;
            }
            
            // In the general case this is not a const operation,
            // as the Map only maps data, but does not copy it.
            // But in our case it is fine, as we never change the Map.
            const Map<MatrixXf> surfaceVerticesInColumns(const_cast<float*>(object.surfaceVertices.data()), 3, numVertices);
            
            return surfaceVerticesInColumns - centers;
        }
        
        MatrixXf Explosion::calculateSqDistancesFromCenter(const MatrixXf& distanceVectors) const {
            return distanceVectors.colwise().squaredNorm();
        }
        
        Matrix<float, Dynamic, 1> Explosion::mapAffectedForcesToSurface(const MatrixXf& sqDistances, const MatrixXf& affectedForceVectors, const ObjectProperties& object) const {
            Matrix<float, Dynamic, 1> forceVectors(object.surfaceVertices.rows(), 1);
            int indexOfAffected = 0;
            for(int i = 0; i < object.surfaceVertices.rows(); i += 3) {
                int indexVector = i / 3;
                if(indexOfAffected >= affectedForceVectors.cols() || sqDistances(0, indexVector) == 0) {
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
        
        MatrixXf Explosion::calculateAffectedParameters(const ObjectProperties& object, MatrixXf& sqDistances, const MatrixXf& distanceVectors) const {
            MatrixXf affectedForceVectors(3, distanceVectors.cols());
            MatrixXf affectedDistances(distanceVectors.cols(), 1);
            MatrixXf affectedSurfaceAreas(distanceVectors.cols(), 1);
            
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
            
            affectedForceVectors.resize(3, numAffectedVectors);
            affectedDistances.resize(numAffectedVectors, 1);
            affectedDistances.cwiseSqrt();
            
            for(int i = 0; i < numAffectedVectors; ++i) {
                // normalize and multiply by force
                affectedForceVectors.row(i) *= (
                            this->calculatePressureAtDistance(affectedDistances(i)) * affectedSurfaceAreas(i)
                        ) / affectedDistances(i);
            }
            
            return affectedForceVectors;
        }

        Matrix<float, Dynamic, 1> Explosion::getForceOnVertices(const ObjectProperties& object) {
            if(this->secondsSinceStart < 0) {
                return MatrixXf(0, 1);
            }
            
            this->setTime(this->secondsSinceStart);
            
            auto distanceVectors = this->calculateDistancesVectorsFromCenter(object);
            auto sqDistances = this->calculateSqDistancesFromCenter(distanceVectors);
            
            MatrixXf affectedForceVectors = this->calculateAffectedParameters(object, sqDistances, distanceVectors);
            if(affectedForceVectors.rows() == 0) {
                return MatrixXf(0, 1);
            }
            
            return object.mapSurfaceForcesToAllVertices(this->mapAffectedForcesToSurface(sqDistances, affectedForceVectors, object));
        }
    }
}
