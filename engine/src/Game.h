#ifndef GAME_H
#define GAME_H

//#include <easylogging++.h>

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/MessageHandler.h"
#include "Window.h"

namespace engine {
    class Game {
    private:
        bool running = false;
        const double ups;
        bool aboutToClose = false; // Note: Probably temporary, used for testing
        
    protected:
        Window window{1024,768};
        ECS::EntityManager entityManager;
        ECS::SystemManager systemManager;
        ECS::MessageHandler messageHandler;

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

