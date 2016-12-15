#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <fstream>

#include "engine/ECS/EntityManager.h"

#include "level.pb.h"

namespace explosionBoy {
    namespace IO {
        class LevelLoader {
        protected:
            std::ifstream levelFile;
            
            void createOuterWalls(const Level& level) const;

        public:
            LevelLoader(const std::string& filename);

            void createLevelEntities(engine::ECS::EntityManager& em);
        };
    }
}

#endif /* LEVELLOADER_H */

