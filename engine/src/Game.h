#ifndef GAME_H
#define GAME_H

#include <easylogging++.h>

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"

namespace engine {
    class Game {
    private:
        bool running = false;
        const double ups;
        bool aboutToClose = false; // Note: Probably temporary, used for testing
        
    protected:
        ECS::EntityManager entityManager;
        ECS::SystemManager systemManager;
        
        virtual void render() = 0;
        virtual void update() = 0;
        virtual void processEvents() = 0;
    public:
        Game(int argc, char** argv, double ups);
        Game(const Game& orig) = delete;
        virtual ~Game();
        
        virtual void initialize();
        virtual void shutdown();
        
        virtual void run();
        
        bool isRunning() const;
        const int getUPS() const;
        bool isAboutToClose() const;
        void setAboutToClose(const bool aboutToClose);
    };
}

#endif /* GAME_H */

