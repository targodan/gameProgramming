#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <fstream>
#include <memory>

#include "engine/ECS/EntityManager.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/Vertex.h"
#include "engine/renderer/Material.h"
#include "engine/renderer/ShaderProgram.h"

#include "pb/level.pb.h"
#include "Grid.h"
#include "Level.h"

namespace explosionBoy {
    class LevelLoader {
    protected:
        pb::Level level;
        Grid grid;
        
        std::shared_ptr<engine::renderer::ShaderProgram> outerWallShader;
        std::shared_ptr<engine::renderer::ShaderProgram> innerWallShader;
        std::shared_ptr<engine::renderer::ShaderProgram> pillarShader;
        
        void createShaderPrograms();
        
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
        
        LevelLoader(std::istream& stream);
        LevelLoader(const std::string& json);
        
        Level createLevel(engine::ECS::EntityManager& em);
        
    public:
        static Level loadLevel(std::istream& stream, engine::ECS::EntityManager& em) {
            LevelLoader ll(stream);
            return ll.createLevel(em);
        }
        
        static Level loadLevel(const std::string& json, engine::ECS::EntityManager& em) {
            LevelLoader ll(json);
            return ll.createLevel(em);
        }
    };
}

#endif /* LEVELLOADER_H */

