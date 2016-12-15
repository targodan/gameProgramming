#include "LevelLoader.h"

#include "engine/IO/SerializerFactory.h"
#include "engine/util/vec3.h"

#include "Grid.h"

using namespace engine::util;
using namespace engine::IO;
using namespace engine::renderer;

namespace explosionBoy {
    LevelLoader::LevelLoader(const std::string& filename) {
        this->levelFile.exceptions(std::ios::failbit);
        this->levelFile.open(filename);
        
        SerializerFactory::humanReadableSerializer().deserialize(this->level, this->levelFile);
        this->grid = Grid(this->level.grid());
    }

    void LevelLoader::createLevelEntities(engine::ECS::EntityManager& em) {
        this->createOuterWallsMesh();
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

    engine::renderer::Mesh LevelLoader::createPillarMesh(size_t x, size_t y) const {
    }
}
