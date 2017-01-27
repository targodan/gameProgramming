#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Force.h"
#include "constants.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        class Explosion : public Force {
            struct AffectedParameters {
                MatrixXf affectedForceVectors;
                MatrixXf affectedDistances;
                MatrixXf affectedSurfaceAreas;
            };
            
        protected:
            Matrix<float, 3, 1> center;
            float tntEquivalence; // in kg
            float expansionSpeed; // in m/s
            float secondsFromExplosion = 0; // in s
            float lastSecondsFromExplosion = 0; // in s
            
            float calculatePressureAtDistance(float d) const; // in Pa
            float calculateExpansionRadius(float secondsFromExplosion) const; // in m
            
            MatrixXf calculateDistancesVectorsFromCenter(const ObjectProperties& object) const;
            VectorXf calculateSqDistancesFromCenter(const MatrixXf& distanceVectors) const;
            MatrixXf calculateAffectedParameters(const ObjectProperties& object, VectorXf& sqDistances, const MatrixXf& distanceVectors) const;
            
            VectorXf mapAffectedForcesToSurface(const MatrixXf& sqDistances, const MatrixXf& affectedForceVectors, const ObjectProperties& object) const;
            
        public:
            Explosion(Matrix<float, 3, 1> center, float tntEquivalence, float expansionSpeed = SPEED_OF_SOUND_IN_AIR)
                : center(center), tntEquivalence(tntEquivalence), expansionSpeed(expansionSpeed) {}
            Explosion(const Explosion& orig)
                : center(orig.center), tntEquivalence(orig.tntEquivalence), expansionSpeed(orig.expansionSpeed),
                    secondsFromExplosion(orig.secondsFromExplosion), lastSecondsFromExplosion(orig.lastSecondsFromExplosion) {}
            Explosion(Explosion&& orig)
                : center(std::move(orig.center)), tntEquivalence(std::move(orig.tntEquivalence)), expansionSpeed(std::move(orig.expansionSpeed)),
                    secondsFromExplosion(std::move(orig.secondsFromExplosion)), lastSecondsFromExplosion(std::move(orig.lastSecondsFromExplosion)) {}
            
            Explosion& operator=(const Explosion& orig);
            Explosion& operator=(Explosion&& orig);
            
            void setTime(float secondsFromExplosion);
            VectorXf getForceOnVertices(const ObjectProperties& object) override;
        };
    }
}

#endif /* EXPLOSION_H */

