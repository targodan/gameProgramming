#include "LevelLoader.h"

#include "engine/IO/SerializerFactory.h"
#include "engine/util/vec3.h"

using namespace engine::util;
using namespace engine::IO;

namespace explosionBoy {
    namespace IO {
        LevelLoader::LevelLoader(const std::string& filename) {
            this->levelFile.exceptions(std::ios::failbit);
            this->levelFile.open(filename);
        }

        void LevelLoader::createLevelEntities(engine::ECS::EntityManager& em) {
            Level level;
            SerializerFactory::humanReadableSerializer().deserialize(level, this->levelFile);
            
            this->createOuterWalls(level);
        }
        
        void LevelLoader::createOuterWalls(const Level& level) const {
            vec3 lowerNorthWest(0, 0, 0);
            vec3 upperNorthWest(0, 0, level.ceiling_height());
            
            vec3 lowerNorthEast(level.cell_size_x() * level.num_cells_x() + level.wall_thickness() * (level.num_cells_x() - 1), lowerNorthWest.y, 0);
            vec3 upperNorthEast(lowerNorthEast.x, lowerNorthEast.y, level.ceiling_height());
            
            vec3 lowerSouthEast(lowerNorthEast.x, level.cell_size_y() * level.num_cells_y() + level.wall_thickness() * (level.num_cells_y() - 1), 0);
            vec3 upperNorthEast(lowerSouthEast.x, lowerSouthEast.y, level.ceiling_height());
            
            vec3 lowerSouthWest(lowerNorthWest.x, lowerSouthEast.y, 0);
            vec3 upperSouthWest(lowerSouthWest.x, lowerSouthWest.y, level.ceiling_height());
            
            // return mesh;
        }
    }
}
