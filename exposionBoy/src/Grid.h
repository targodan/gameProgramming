#ifndef GRID_H
#define GRID_H

#include "engine/util/vec3.h"

#include "pb/level.pb.h"

namespace explosionBoy {
    class Grid {
        using vec3 = engine::util::vec3;
        
    protected:
        pb::Grid grid;
        
    public:
        Grid() {}
        Grid(const pb::Grid& grid) : grid(grid) {}
        
        void setNumCellsX(uint64_t x) {
            this->grid.set_num_cells_x(x);
        }
        
        void setNumCellsY(uint64_t y) {
            this->grid.set_num_cells_y(y);
        }
        
        void setCellWidth(float width) {
            this->grid.set_cell_width(width);
        }
        
        void setCellHeight(float height) {
            this->grid.set_cell_height(height);
        }
        
        void setCeilingHeight(float height) {
            this->grid.set_ceiling_height(height);
        }
        
        void setWallThickness(float thickness) {
            this->grid.set_wall_thickness(thickness);
        }
        
        void setPillarThickness(float thickness) {
            return this->grid.set_pillar_thickness(thickness);
        }
        
        uint64_t getNumCellsX() const {
            return this->grid.num_cells_x();
        }
        
        uint64_t getNumCellsY() const {
            return this->grid.num_cells_y();
        }
        
        float getCellWidth() const {
            return this->grid.cell_width();
        }
        
        float getCellHeight() const {
            return this->grid.cell_height();
        }
        
        float getCeilingHeight() const {
            return this->grid.ceiling_height();
        }
        
        float getWallThickness() const {
            return this->grid.wall_thickness();
        }
        
        float getPillarThickness() const {
            return this->grid.pillar_thickness();
        }
        
        class WorldCoordProxy {
        protected:
            const Grid& grid;
            uint64_t x;
            uint64_t y;
            
        public:
            WorldCoordProxy(const Grid& grid, uint64_t x, uint64_t y) : grid(grid), x(x), y(y) {}
            
            vec3 northWestCorner() const {
                return vec3(
                        this->grid.getCellWidth() * this->x + this->grid.getWallThickness() * (this->x == 0 ? 0 : this->x - 1),
                        0,
                        this->grid.getCellHeight() * this->y + this->grid.getWallThickness() * (this->y == 0 ? 0 : this->y - 1)
                    );
            }
            
            vec3 northEastCorner() const {
                return this->northWestCorner() + vec3(this->grid.getCellWidth(), 0, 0);
            }
            
            vec3 southWestCorner() const {
                return this->northWestCorner() + vec3(0, 0, this->grid.getCellHeight());
            }
            
            vec3 southEastCorner() const {
                return this->northWestCorner() + vec3(this->grid.getCellWidth(), 0, this->grid.getCellHeight());
            }
            
            vec3 center() const {
                return this->northWestCorner() + vec3(this->grid.getCellWidth() / 2., 0, this->grid.getCellHeight() / 2.);
            }
        };
        
        const WorldCoordProxy cellToWorldCoords(uint64_t x, uint64_t y) const {
            return WorldCoordProxy(*this, x, y);
        }
    };
}

#endif /* GRID_H */

