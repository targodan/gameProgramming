#include "LevelLoader.h"

#include "engine/IO/SerializerFactory.h"
#include "engine/util/vec3.h"
#include "engine/ECS/Entity.h"
#include "engine/ECSCommon/VisualComponent.h"

#include "Grid.h"
#include "FormatException.h"

using namespace engine::util;
using namespace engine::IO;
using namespace engine::renderer;
using namespace engine::ECSCommon;

namespace explosionBoy {
    LevelLoader::LevelLoader(std::istream& stream) {
        SerializerFactory::humanReadableSerializer().deserialize(this->level, stream);
        this->grid = Grid(this->level.grid());
    }
    
    LevelLoader::LevelLoader(const std::string& json) {
        SerializerFactory::humanReadableSerializer().deserialize(this->level, json);
        this->grid = Grid(this->level.grid());
    }
    
    void LevelLoader::checkWallsString() const {
        if(static_cast<uint64_t>(this->level.walls().size()) != this->grid.getNumCellsY()*2-1) {
            throw FormatException("Invalid number of wall-string-rows! With %ul cells in y, you should have %ul rows in the wall-string.", this->grid.getNumCellsY(), this->grid.getNumCellsY()*2-1);
        }
        uint64_t i = 1;
        for(auto& row : this->level.walls()) {
            if(row.length() != this->grid.getNumCellsX()*2-1) {
                throw FormatException("Invalid number of characters in wall-string no. %ul! With %ul cells in x, you should have %ul characters in the each wall-string.", i, this->grid.getNumCellsX(), this->grid.getNumCellsX()*2-1);
            }
            ++i;
        }
        
        for(uint64_t y = 0; y < this->grid.getNumCellsY()*2-1; y++) {
            for(uint64_t x = 0; x < this->grid.getNumCellsX()*2-1; x++) {
                if(y % 2 == 0) {
                    // Definition of the north-south walls.
                    if(x % 2 == 0) {
                        // This is a cell.
                        if(this->level.walls(y).at(x) != ' ') {
                            throw FormatException("Wall definition row %ul char %ul represents a cell and should thus be ' '.", y, x);
                        }
                    } else {
                        // This is a wall.
                        int type;
                        if(this->level.walls(y).at(x) == ' ') {
                            type = 0;
                        } else {
                            type = static_cast<int>(this->level.walls(y).at(x)) + '0';
                        }
                        if(!pb::WallType_IsValid(type)) {
                            throw FormatException("Wall definition row %ul char %ul represents a wall and should thus be ' ' or '0' - '%d'.", y, x, pb::WallType_MAX);
                        }
                    }
                } else {
                    // Definition of the west-east walls.
                    if(x % 2 == 0) {
                        // This is a wall.
                        int type;
                        if(this->level.walls(y).at(x) == ' ') {
                            type = 0;
                        } else {
                            type = static_cast<int>(this->level.walls(y).at(x)) + '0';
                        }
                        if(!pb::WallType_IsValid(type)) {
                            throw FormatException("Wall definition row %ul char %ul represents a wall and should thus be ' ' or '0' - '%d'.", y, x, pb::WallType_MAX);
                        }
                    } else {
                        // This is a pillar.
                        if(this->level.walls(y).at(x) != 'x') {
                            throw FormatException("Wall definition row %ul char %ul represents a cell and should thus be 'x'.", y, x);
                        }
                    }
                }
            }
        }
    }
    
    pb::WallType LevelLoader::getWallTypeEastOfCell(size_t x, size_t y) const {
        int type;
        char wallTypeChar = this->level.walls(y*2).at(x*2+1);
        if(wallTypeChar == ' ') {
            type = 0;
        } else {
            type = static_cast<int>(wallTypeChar) - '0';
        }
        return static_cast<pb::WallType>(type);
    }
    
    pb::WallType LevelLoader::getWallTypeSouthOfCell(size_t x, size_t y) const {
        int type;
        char wallTypeChar = this->level.walls(y*2+1).at(x*2);
        if(wallTypeChar == ' ') {
            type = 0;
        } else {
            type = static_cast<int>(wallTypeChar) - '0';
        }
        return static_cast<pb::WallType>(type);
    }
    
    void LevelLoader::createShaderPrograms() {
        this->innerWallShader = std::make_shared<ShaderProgram>(this->level.inner_wall_shader()+".vsh", this->level.inner_wall_shader()+".fsh");
        this->outerWallShader = std::make_shared<ShaderProgram>(this->level.outer_wall_shader()+".vsh", this->level.outer_wall_shader()+".fsh");
        this->pillarShader = std::make_shared<ShaderProgram>(this->level.pillar_shader()+".vsh", this->level.pillar_shader()+".fsh");
    }

    std::unique_ptr<Level> LevelLoader::createLevel(engine::ECS::EntityManager& em) {
        this->createShaderPrograms();
        
        auto outerWallEntity = em.createEntity("OuterWall")
                .addComponent<VisualComponent>(this->createOuterWallsMesh(), Material(this->outerWallShader));
        
        engine::util::vector<Entity> innerWallEntities;
        engine::util::vector<Entity> pillarEntities;
        
        std::stringstream ss;
        for(uint64_t y = 0; y < this->grid.getNumCellsY(); ++y) {
            for(uint64_t x = 0; x < this->grid.getNumCellsX(); ++x) {
                auto wallType = this->getWallTypeEastOfCell(x, y);
                if(x+1 < this->grid.getNumCellsX() && wallType != pb::NONE) {
                    auto mesh = this->createWallMeshEastOfCell(x, y);
                    ss << "InnerWall_EastOf_" << x << "_" << y;
                    innerWallEntities.push_back(
                            em.createEntity(ss.str())
                            .addComponent<VisualComponent>(mesh, Material(this->innerWallShader))
                            );
                    ss.str("");
                }
                if(y+1 < this->grid.getNumCellsY() && wallType != pb::NONE) {
                    auto mesh = this->createWallMeshSouthOfCell(x, y);
                    ss << "InnerWall_SouthOf_" << x << "_" << y;
                    innerWallEntities.push_back(
                            em.createEntity(ss.str())
                            .addComponent<VisualComponent>(mesh, Material(this->innerWallShader))
                            );
                    ss.str("");
                }
                if(x != 0 || y != 0) {
                    auto mesh = this->createPillarMeshNorthWest(x, y);
                    ss << "Pillar_NorthWestOf_" << x << "_" << y;
                    pillarEntities.push_back(
                            em.createEntity(ss.str())
                                    .addComponent<VisualComponent>(mesh, Material(this->pillarShader))
                            );
                    ss.str("");
                }
            }
        }
        for(uint64_t y = 1; y < this->grid.getNumCellsY(); ++y) {
            uint64_t x = this->grid.getNumCellsX()-1;
            auto mesh = this->createPillarMeshNorthEast(x, y);
            ss << "Pillar_NorthEastOf_" << x << "_" << y;
            pillarEntities.push_back(
                    em.createEntity(ss.str())
                            .addComponent<VisualComponent>(mesh, Material(this->pillarShader))
                    );
            ss.str("");
        }
        for(uint64_t x = 1; x < this->grid.getNumCellsX(); ++x) {
            uint64_t y = this->grid.getNumCellsY()-1;
            auto mesh = this->createPillarMeshSouthWest(x, y);
            ss << "Pillar_SouthWestOf_" << x << "_" << y;
            pillarEntities.push_back(
                    em.createEntity(ss.str())
                            .addComponent<VisualComponent>(mesh, Material(this->pillarShader))
                    );
            ss.str("");
        }
        
        return std::make_unique<Level>(outerWallEntity, innerWallEntities, pillarEntities);
    }

    engine::renderer::Mesh LevelLoader::createOuterWallsMesh() const {
        Vertex lowerNorthWest(this->grid.cellToWorldCoords(0, 0).northWestCorner(), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperNorthWest(vec3(lowerNorthWest.position.x, this->grid.getCeilingHeight(), lowerNorthWest.position.z), vec3(0, 0, 0), vec2(0, 0));

        auto v = this->grid.cellToWorldCoords(this->grid.getNumCellsX()-1, 0).northWestCorner();
        v.x += this->grid.getCellWidth();
        Vertex lowerNorthEast(v, vec3(0, 0, 0), vec2(0, 0));
        Vertex upperNorthEast(vec3(lowerNorthEast.position.x, this->grid.getCeilingHeight(), lowerNorthEast.position.z), vec3(0, 0, 0), vec2(0, 0));

        v = this->grid.cellToWorldCoords(0, this->grid.getNumCellsY()-1).northWestCorner();
        v.z += this->grid.getCellHeight();
        Vertex lowerSouthWest(v, vec3(0, 0, 0), vec2(0, 0));
        Vertex upperSouthWest(vec3(lowerSouthWest.position.x, this->grid.getCeilingHeight(), lowerSouthWest.position.z), vec3(0, 0, 0), vec2(0, 0));
        
        Vertex lowerSouthEast(vec3(lowerNorthEast.position.x, 0, lowerSouthWest.position.z), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperSouthEast(vec3(lowerSouthEast.position.x, this->grid.getCeilingHeight(), lowerSouthEast.position.z), vec3(0, 0, 0), vec2(0, 0));

        return Mesh({
                    lowerNorthWest /*0*/, upperNorthWest /*1*/,
                    lowerNorthEast /*2*/, upperNorthEast /*3*/,
                    lowerSouthEast /*4*/, upperSouthEast /*5*/,
                    lowerSouthWest /*6*/, upperSouthWest /*7*/
                },{
                    // Northern wall
                    1, 0, 2,
                    1, 2, 3,

                    // Eastern wall
                    3, 2, 4,
                    3, 4, 5,

                    // Southern wall
                    5, 4, 6,
                    5, 6, 7,

                    // Western wall
                    7, 6, 0,
                    7, 0, 1
                });
    }
    
    engine::renderer::Mesh LevelLoader::createPillarMeshNorthWest(size_t x, size_t y) const {
        const auto pos = this->grid.cellToWorldCoords(x, y).northWestCorner();
        float wallHalf = this->grid.getWallThickness() / 2.;
        return this->createPillarMesh(pos.x - wallHalf, pos.z - wallHalf);
    }
    
    engine::renderer::Mesh LevelLoader::createPillarMeshNorthEast(size_t x, size_t y) const {
        const auto pos = this->grid.cellToWorldCoords(x, y).northEastCorner();
        float wallHalf = this->grid.getWallThickness() / 2.;
        return this->createPillarMesh(pos.x + wallHalf, pos.z - wallHalf);
    }
    
    engine::renderer::Mesh LevelLoader::createPillarMeshSouthWest(size_t x, size_t y) const {
        const auto pos = this->grid.cellToWorldCoords(x, y).southWestCorner();
        float wallHalf = this->grid.getWallThickness() / 2.;
        return this->createPillarMesh(pos.x - wallHalf, pos.z + wallHalf);
    }
    
    engine::renderer::Mesh LevelLoader::createPillarMeshSouthEast(size_t x, size_t y) const {
        const auto pos = this->grid.cellToWorldCoords(x, y).southEastCorner();
        float wallHalf = this->grid.getWallThickness() / 2.;
        return this->createPillarMesh(pos.x + wallHalf, pos.z + wallHalf);
    }
    
    engine::renderer::Mesh LevelLoader::createPillarMesh(float centerX, float centerZ) const {
        const float halfSize = this->grid.getPillarThickness() / 2.;
        Vertex lowerNorthWest(vec3(centerX - halfSize, 0, centerZ - halfSize), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperNorthWest(vec3(lowerNorthWest.position.x, this->grid.getCeilingHeight(), lowerNorthWest.position.z), vec3(0, 0, 0), vec2(0, 0));
        
        Vertex lowerNorthEast(vec3(centerX + halfSize, 0, centerZ - halfSize), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperNorthEast(vec3(lowerNorthEast.position.x, this->grid.getCeilingHeight(), lowerNorthEast.position.z), vec3(0, 0, 0), vec2(0, 0));
        
        Vertex lowerSouthWest(vec3(centerX - halfSize, 0, centerZ + halfSize), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperSouthWest(vec3(lowerSouthWest.position.x, this->grid.getCeilingHeight(), lowerSouthWest.position.z), vec3(0, 0, 0), vec2(0, 0));
        
        Vertex lowerSouthEast(vec3(centerX + halfSize, 0, centerZ + halfSize), vec3(0, 0, 0), vec2(0, 0));
        Vertex upperSouthEast(vec3(lowerSouthEast.position.x, this->grid.getCeilingHeight(), lowerSouthEast.position.z), vec3(0, 0, 0), vec2(0, 0));
        
        return Mesh({
                    lowerNorthWest /*0*/, upperNorthWest /*1*/,
                    lowerNorthEast /*2*/, upperNorthEast /*3*/,
                    lowerSouthEast /*4*/, upperSouthEast /*5*/,
                    lowerSouthWest /*6*/, upperSouthWest /*7*/
                },{
                    // Northern wall
                    2, 0, 1,
                    3, 2, 1,

                    // Eastern wall
                    4, 2, 3,
                    5, 4, 3,

                    // Southern wall
                    6, 4, 5,
                    7, 6, 5,

                    // Western wall
                    0, 6, 7,
                    1, 0, 7
                });
    }
    
    engine::renderer::Mesh LevelLoader::createWallMeshEastOfCell(size_t x, size_t y) const {
        vec3 lowerNorthWest = this->grid.cellToWorldCoords(0, 0).northEastCorner();
        vec3 upperNorthWest = vec3(lowerNorthWest.x, this->grid.getCeilingHeight(), lowerNorthWest.z);
        
        vec3 lowerNorthEast = vec3(lowerNorthWest.x + this->grid.getWallThickness(), 0, lowerNorthWest.z);
        vec3 upperNorthEast = vec3(lowerNorthEast.x, this->grid.getCeilingHeight(), lowerNorthEast.z);
        
        vec3 lowerSouthEast = vec3(lowerNorthEast.x, 0, lowerNorthEast.z + this->grid.getWallThickness());
        vec3 upperSouthEast = vec3(lowerSouthEast.x, this->grid.getCeilingHeight(), lowerSouthEast.z);
        
        vec3 lowerSouthWest = vec3(lowerNorthWest.x, 0, lowerSouthEast.z);
        vec3 upperSouthWest = vec3(lowerSouthWest.x, this->grid.getCeilingHeight(), lowerSouthWest.z);
        
        return Mesh({
                    lowerNorthWest /*0*/, upperNorthWest /*1*/,
                    lowerNorthEast /*2*/, upperNorthEast /*3*/,
                    lowerSouthEast /*4*/, upperSouthEast /*5*/,
                    lowerSouthWest /*6*/, upperSouthWest /*7*/
                },{
                    // Northern wall
                    2, 0, 1,
                    3, 2, 1,

                    // Eastern wall
                    4, 2, 3,
                    5, 4, 3,

                    // Southern wall
                    6, 4, 5,
                    7, 6, 5,

                    // Western wall
                    0, 6, 7,
                    1, 0, 7
                });
    }
    
    engine::renderer::Mesh LevelLoader::createWallMeshSouthOfCell(size_t x, size_t y) const {
        vec3 lowerNorthWest = this->grid.cellToWorldCoords(0, 0).northEastCorner();
        vec3 upperNorthWest = vec3(lowerNorthWest.x, this->grid.getCeilingHeight(), lowerNorthWest.z);
        
        vec3 lowerNorthEast = vec3(lowerNorthWest.x + this->grid.getWallThickness(), 0, lowerNorthWest.z);
        vec3 upperNorthEast = vec3(lowerNorthEast.x, this->grid.getCeilingHeight(), lowerNorthEast.z);
        
        vec3 lowerSouthEast = vec3(lowerNorthEast.x, 0, lowerNorthEast.z + this->grid.getWallThickness());
        vec3 upperSouthEast = vec3(lowerSouthEast.x, this->grid.getCeilingHeight(), lowerSouthEast.z);
        
        vec3 lowerSouthWest = vec3(lowerNorthWest.x, 0, lowerSouthEast.z);
        vec3 upperSouthWest = vec3(lowerSouthWest.x, this->grid.getCeilingHeight(), lowerSouthWest.z);
        
        return Mesh({
                    lowerNorthWest /*0*/, upperNorthWest /*1*/,
                    lowerNorthEast /*2*/, upperNorthEast /*3*/,
                    lowerSouthEast /*4*/, upperSouthEast /*5*/,
                    lowerSouthWest /*6*/, upperSouthWest /*7*/
                },{
                    // Northern wall
                    2, 0, 1,
                    3, 2, 1,

                    // Eastern wall
                    4, 2, 3,
                    5, 4, 3,

                    // Southern wall
                    6, 4, 5,
                    7, 6, 5,

                    // Western wall
                    0, 6, 7,
                    1, 0, 7
                });
    }
}
