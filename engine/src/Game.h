#ifndef GAME_H
#define GAME_H

#include <easylogging++.h>

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/MessageHandler.h"

namespace engine {
    class Game {
    protected:
        ECS::EntityManager entityManager;
        ECS::SystemManager systemManager;
        ECS::MessageHandler messageHandler;

    public:
        Game(int argc, char** argv);
        Game(const Game& orig) = delete;
        virtual ~Game();
        
        virtual void initialize();
        virtual void run();
        virtual void shutdown();
    };
}

#endif /* GAME_H */

