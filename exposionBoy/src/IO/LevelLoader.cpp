#include "LevelLoader.h"

#include "engine/IO/SerializerFactory.h"
#include "engine/util/vec3.h"

using namespace engine::util;
using namespace engine::IO;
using namespace engine::renderer;

namespace explosionBoy {
    namespace IO {
        LevelLoader::LevelLoader(const std::string& filename) {
            this->levelFile.exceptions(std::ios::failbit);
            this->levelFile.open(filename);
        }

        void LevelLoader::createLevelEntities(engine::ECS::EntityManager& em) {
            Level level;
            SerializerFactory::humanReadableSerializer().deserialize(level, this->levelFile);
            
            this->createOuterWallsMesh(level);
        }
        
        engine::renderer::Mesh LevelLoader::createOuterWallsMesh(const Level& level) const {
            Vertex lowerNorthWest(vec3(0, 0, 0), vec3(0, 0, 0), vec2(0, 0));
            Vertex upperNorthWest(vec3(0, 0, level.ceiling_height()), vec3(0, 0, 0), vec2(0, 0));
            
            Vertex lowerNorthEast(vec3(level.cell_size_x() * level.num_cells_x() + level.wall_thickness() * (level.num_cells_x() - 1), lowerNorthWest.position.y, 0), vec3(0, 0, 0), vec2(0, 0));
            Vertex upperNorthEast(vec3(lowerNorthEast.position.x, lowerNorthEast.position.y, level.ceiling_height()), vec3(0, 0, 0), vec2(0, 0));
            
            Vertex lowerSouthEast(vec3(lowerNorthEast.position.x, level.cell_size_y() * level.num_cells_y() + level.wall_thickness() * (level.num_cells_y() - 1), 0), vec3(0, 0, 0), vec2(0, 0));
            Vertex upperSouthEast(vec3(lowerSouthEast.position.x, lowerSouthEast.position.y, level.ceiling_height()), vec3(0, 0, 0), vec2(0, 0));
            
            Vertex lowerSouthWest(vec3(lowerNorthWest.position.x, lowerSouthEast.position.y, 0), vec3(0, 0, 0), vec2(0, 0));
            Vertex upperSouthWest(vec3(lowerSouthWest.position.x, lowerSouthWest.position.y, level.ceiling_height()), vec3(0, 0, 0), vec2(0, 0));
            
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
        
        engine::renderer::Mesh LevelLoader::createPillarsMesh(const Level& level, size_t x, size_t y) const {
            
        }
    }
}
