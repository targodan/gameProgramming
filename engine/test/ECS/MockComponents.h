#ifndef MOCKCOMPONENTS_H
#define MOCKCOMPONENTS_H

#include "ECS/Component.h"
#include "ECS/ComponentRegisterer.h"

using engine::ECS::componentId_t;

#define MOCK_COMPONENT(name) \
class name : public engine::ECS::Component { \
private: \
    static componentId_t typeId; \
    size_t someData; \
 \
public: \
    name(size_t data) : someData(data) {} \
    virtual ~name() {} \
     \
    size_t getData() const { \
        return this->someData; \
    } \
    std::string getComponentName() const override { \
        return #name; \
    } \
    std::string toString() const override { \
        return this->getComponentName(); \
    } \
    componentId_t getComponentId() const { \
        return name::typeId; \
    } \
     \
    static void setComponentTypeId(componentId_t id) { \
        name::typeId = id; \
    } \
    static componentId_t getComponentTypeId() { \
        return name::typeId; \
    } \
}; \
componentId_t name::typeId = 0; \
ECS_REGISTER_COMPONENT(name)

MOCK_COMPONENT(Comp1);
MOCK_COMPONENT(Comp2);
MOCK_COMPONENT(Comp3);
MOCK_COMPONENT(Comp4);
MOCK_COMPONENT(Comp5);

#endif /* MOCKCOMPONENTS_H */

