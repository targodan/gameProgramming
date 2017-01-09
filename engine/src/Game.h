#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

//#include <easylogging++.h>

#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "ECS/MessageHandler.h"
#include "Window.h"

namespace engine {
    class Game {
    private:
        bool running = false;
        const double updatesPerSecond;
        bool aboutToClose = false; // Note: Probably temporary, used for testing
        bool shutdownComplete = false;
        
    protected:
        Window window;
        ECS::EntityManager entityManager;
        ECS::SystemManager systemManager;
        ECS::MessageHandler messageHandler;

        virtual void render(double deltaTimeSeconds);
        virtual void update(double deltaTimeSeconds);
        virtual void processEvents();
    
    public:
        Game(int argc, char** argv, double ups=60);
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

