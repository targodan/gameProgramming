#ifndef LEVEL_H
#define LEVEL_H

#include "engine/ECS/Entity.h"
#include "engine/util/Array.h"

namespace explosionBoy {
    using engine::util::Array;
    using engine::ECS::Entity;
    
    class Level {
    protected:
        Entity outerWall;
        Array<Entity> innerWalls;
        Array<Entity> pillars;
        
    public:
        Level(Entity outerWall, const Array<Entity>& innerWalls, const Array<Entity>& pillars)
            : outerWall(outerWall), innerWalls(innerWalls), pillars(pillars) {}
    };
}

#endif /* LEVEL_H */

