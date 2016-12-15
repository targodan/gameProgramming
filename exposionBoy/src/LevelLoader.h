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
        pb::Level level;
        Grid grid;
        
        engine::renderer::Mesh createOuterWallsMesh() const;
        engine::renderer::Mesh createPillarMeshNorthWest(size_t x, size_t y) const;
        engine::renderer::Mesh createPillarMeshNorthEast(size_t x, size_t y) const;
        engine::renderer::Mesh createPillarMeshSouthWest(size_t x, size_t y) const;
        engine::renderer::Mesh createPillarMeshSouthEast(size_t x, size_t y) const;
        engine::renderer::Mesh createPillarMesh(float centerX, float centerZ) const;
        engine::renderer::Mesh createWallMeshEastOfCell(size_t x, size_t y) const;
        engine::renderer::Mesh createWallMeshSouthOfCell(size_t x, size_t y) const;
        
        void checkWallsString() const;
        pb::WallType getWallTypeEastOfCell(size_t x, size_t y) const;
        pb::WallType getWallTypeSouthOfCell(size_t x, size_t y) const;
        
    public:
        LevelLoader(std::ifstream& file);
        LevelLoader(const std::string& json);
        
        void createLevelEntities(engine::ECS::EntityManager& em);
    };
}

#endif /* LEVELLOADER_H */

