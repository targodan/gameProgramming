#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>

#include "Component.h"

using std::vector;
using std::shared_ptr;

typedef unsigned int uuid;
    
class EntityManager {
private:
    vector<shared_ptr<Component>> components;
    uuid nextId;
    
public:
    EntityManager();
    EntityManager(const EntityManager& orig);
    virtual ~EntityManager();
};

#endif /* ENTITYMANAGER_H */

