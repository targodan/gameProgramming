#ifndef MOCKCOMPONENTS_H
#define MOCKCOMPONENTS_H

#include "ECS/SerializableComponent.h"
#include "ECS/ComponentRegisterer.h"

#include "pb/mockcomponents.pb.h"

#include <iostream>

using engine::ECS::componentId_t;

#define MOCK_COMPONENT(name) \
class name : public engine::ECS::SerializableComponent { \
private: \
    static componentId_t typeId; \
    pb::name msg; \
     \
public: \
    name() { msg.set_data(0); } \
    name(size_t data) { msg.set_data(data); } \
    virtual ~name() {} \
     \
    size_t getData() const { \
        return this->msg.data(); \
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
    google::protobuf::Message& fromProtobufMessage() override {return this->msg;} \
    const google::protobuf::Message& toProtobufMessage() override {return this->msg;} \
    void afterProtobufMessageUpdate() {} \
     \
    static void setComponentTypeId(componentId_t id) { \
        name::typeId = id; \
    } \
    static componentId_t getComponentTypeId() { \
        return name::typeId; \
    } \
}

MOCK_COMPONENT(Comp1);
MOCK_COMPONENT(Comp2);
MOCK_COMPONENT(Comp3);
MOCK_COMPONENT(Comp4);
MOCK_COMPONENT(Comp5);


class SortComponent : public engine::ECS::Component {
private:
    static componentId_t typeId;
    size_t order;

public:
    SortComponent() : order(0) {}
    SortComponent(size_t order) : order(order) {}
    virtual ~SortComponent() {}
    
    size_t getOrder() const {
        return this->order;
    }
    std::string getComponentName() const override {
        return "SortComponent";
    }
    std::string toString() const override {
        return this->getComponentName();
    }
    componentId_t getComponentId() const {
        return SortComponent::typeId;
    }
    
    static void setComponentTypeId(componentId_t id) {
        SortComponent::typeId = id;
    }
    static componentId_t getComponentTypeId() {
        return SortComponent::typeId;
    }
};

#endif /* MOCKCOMPONENTS_H */

