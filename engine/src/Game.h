#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

//#include <easylogging++.h>

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/MessageHandler.h"
#include "Window.h"

namespace engine {
    class Game {
    protected:
        bool running = false;
        const float updatesPerSecond;
        bool aboutToClose = false; // Note: Probably temporary, used for testing
        bool shutdownComplete = false;
        int openmpThreads = -1;
        
        ECS::MessageHandler messageHandler;
        
        Window window;
        ECS::EntityManager entityManager;
        ECS::SystemManager systemManager;

        virtual void render(float deltaTimeSeconds);
        virtual void update(float deltaTimeSeconds);
        virtual void processEvents();
    
    public:
        Game(int argc, char** argv, float ups=60);
        Game(const Game& orig) = delete;
        virtual ~Game();
        
        virtual void initialize();
        virtual void shutdown();
        
        virtual void run();
        
        bool isRunning() const;
        const int getUpdatesPerSecond() const;
        bool isAboutToClose() const;
        void setAboutToClose(const bool aboutToClose);
    };
}

#endif /* ENGINE_GAME_H */

