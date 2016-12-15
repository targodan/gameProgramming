#ifndef DEMO02_H
#define DEMO02_H

#include "Game.h"

namespace demo {
    class Demo02 : public engine::Game {
    public:
        Demo02(int argc, char** argv, double ups=60);
        Demo02(const Demo02& orig) = delete;
        virtual ~Demo02();

        void processEvents() override;
        void render() override;
        void update() override;
    private:

    };
}

#endif /* DEMO02_H */

