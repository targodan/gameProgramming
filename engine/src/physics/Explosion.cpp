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
        
        MatrixXf Explosion::calculateDistancesVectorsFromCenter(const Surface& surface) const {
            Index numVertices = surface.vertices.rows() / 3;
            MatrixXf centers(numVertices, 3);
            for(int i = 0; i < numVertices; i += 3) {
                centers.row(i) = this->center;
            }
            
            // In the general case this is not a const operation,
            // as the Map only maps data, but does not copy it.
            // But in our case it is fine, as we never change the Map.
            const Map<MatrixXf> surfaceVerticesInColumns(const_cast<float*>(surface.vertices.data()), numVertices, 3);
            
            return surfaceVerticesInColumns - centers;
        }
        
        MatrixXf Explosion::calculateSqDistancesFromCenter(const MatrixXf& distanceVectors) const {
            return distanceVectors.rowwise().squaredNorm();
        }
        
        Matrix<float, Dynamic, 1> Explosion::mapAffectedForcesToSurface(const MatrixXf& sqDistances, const MatrixXf& affectedForceVectors, const Surface& surface) const {
            Matrix<float, Dynamic, 1> forceVectors(surface.vertices.rows(), 1);
            int indexOfAffected = 0;
            for(int i = 0; i < surface.vertices.rows(); i += 3) {
                int indexVector = i * 3;
                if(sqDistances(indexVector) == 0) {
                    forceVectors(i+0) = 0;
                    forceVectors(i+1) = 0;
                    forceVectors(i+2) = 0;
                } else {
                    forceVectors(i+0) = affectedForceVectors(indexOfAffected, 0);
                    forceVectors(i+1) = affectedForceVectors(indexOfAffected, 1);
                    forceVectors(i+2) = affectedForceVectors(indexOfAffected, 2);
                    ++indexOfAffected;
                }
            }
            return forceVectors;
        }
        
        MatrixXf Explosion::calculateAffectedParameters(const Surface& surface, MatrixXf& sqDistances, const MatrixXf& distanceVectors) const {
            MatrixXf affectedForceVectors(distanceVectors.cols(), 3);
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
                    affectedSurfaceAreas(numAffectedVectors) = surface.surfaceAreaPerVertex(i);
                    affectedForceVectors.row(numAffectedVectors) = distanceVectors.row(i);
                    ++numAffectedVectors;
                }
            }
            if(numAffectedVectors == 0) {
                return MatrixXf(0, 0);
            }
            
            affectedForceVectors.resize(numAffectedVectors, 3);
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

        Matrix<float, Dynamic, 1> Explosion::getForceOnVertices(const Surface& surface) const {
            auto distanceVectors = this->calculateDistancesVectorsFromCenter(surface);
            auto sqDistances = this->calculateSqDistancesFromCenter(distanceVectors);
            
            MatrixXf affectedForceVectors = this->calculateAffectedParameters(surface, sqDistances, distanceVectors);
            if(affectedForceVectors.rows() == 0) {
                return MatrixXf(0, 1);
            }
            
            return this->mapAffectedForcesToSurface(sqDistances, affectedForceVectors, surface);
        }
    }
}
