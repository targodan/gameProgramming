#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <fstream>

#include "engine/ECS/EntityManager.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/Vertex.h"

#include "level.pb.h"

namespace explosionBoy {
    namespace IO {
        class LevelLoader {
        protected:
            std::ifstream levelFile;
            
            engine::renderer::Mesh createOuterWallsMesh(const Level& level) const;
            engine::renderer::Mesh createPillarsMesh(const Level& level, size_t x, size_t y) const;

        public:
            LevelLoader(const std::string& filename);

            void createLevelEntities(engine::ECS::EntityManager& em);
        };
    }
}

#endif /* LEVELLOADER_H */

