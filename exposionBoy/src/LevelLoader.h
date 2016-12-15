#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <fstream>

#include "engine/ECS/EntityManager.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/Vertex.h"

#include "pb/level.pb.h"
#include "Grid.h"

namespace explosionBoy {
    class LevelLoader {
    protected:
        std::ifstream levelFile;
        pb::Level level;
        Grid grid;

        engine::renderer::Mesh createOuterWallsMesh() const;
        engine::renderer::Mesh createPillarMesh(size_t x, size_t y) const;

    public:
        LevelLoader(const std::string& filename);

        void createLevelEntities(engine::ECS::EntityManager& em);
    };
}

#endif /* LEVELLOADER_H */

