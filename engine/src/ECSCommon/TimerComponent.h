#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include "../ECS/Component.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        
        class TimerComponent : public Component {
        private:
            static componentId_t typeId;
            
        protected:
            float time;
            
        public:
            TimerComponent(float startInXSeconds = 0) : time(-startInXSeconds) {}
            TimerComponent(const TimerComponent& orig) = delete;
            virtual ~TimerComponent() {}
            
            float getTime() const {
                return this->time;
            }
            
            void addTime(float time) {
                this->time += time;
            }
            
            void reset(float startInXSeconds = 0) {
                this->time = -startInXSeconds;
            }
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* TIMERCOMPONENT_H */

