#include "MockComponents.h"

componentId_t Comp1::typeId = 0;
componentId_t Comp2::typeId = 0;
componentId_t Comp3::typeId = 0;
componentId_t Comp4::typeId = 0;
componentId_t Comp5::typeId = 0;

ECS_REGISTER_COMPONENT(Comp1);
ECS_REGISTER_COMPONENT(Comp2);
ECS_REGISTER_COMPONENT(Comp3);
ECS_REGISTER_COMPONENT(Comp4);
ECS_REGISTER_COMPONENT(Comp5);


componentId_t SortComponent::typeId = 0;
ECS_REGISTER_COMPONENT(SortComponent);