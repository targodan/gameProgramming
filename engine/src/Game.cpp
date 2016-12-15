#include "Game.h"

#include <execinfo.h>
#include <stdlib.h>
#include <iostream>

#include <easylogging++.h>

namespace engine {
    Game::Game(int argc, char** argv, double ups) : updatesPerSecond(ups), entityManager(), systemManager(entityManager) {
        // Configure logger
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Filename, "engine.log");
        defaultConf.set(el::Level::Info, el::ConfigurationType::ToFile, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s} %level : [%logger] %msg");
        el::Loggers::setDefaultConfigurations(defaultConf, true);
        
        START_EASYLOGGINGPP(argc, argv);
        
        LOG(INFO) << "Engine started up.";
        
        // Catch uncaught exceptions and log
        std::set_terminate([] {
            // Retrieve a possibly uncaught exception.
            // TODO: Try-catch really necessary here?
            std::exception_ptr exptr = std::current_exception();
            try {
                std::rethrow_exception(exptr);
            } catch (std::exception &ex) {
                // Really was an uncaught exception.
                LOG(FATAL) << "Uncaught exception: " << ex.what();
                
#ifdef __unix__
                void* trace[256];
                size_t size = backtrace(trace, 256);
                char** strings = backtrace_symbols(trace, size);

                LOG(FATAL) << "Stacktrace:";
                for(size_t i = 0; i < size; ++i) {
                    LOG(FATAL) << i << ": " << strings[i];
                }
                free(strings);
#endif
            }
            LOG(INFO) << "Execution aborted.";
            std::abort();
        });
    }
    
    Game::~Game() {
        this->shutdown();
        LOG(INFO) << "Engine shut down.";
    }

    void Game::initialize() {
        this->systemManager.setup();
    }
    
    void Game::render(double deltaTimeSeconds) {
        this->systemManager.render(deltaTimeSeconds);
    }
    
    void Game::update(double deltaTimeSeconds) {
        this->systemManager.update(deltaTimeSeconds);
    }
    
    void Game::processEvents() {
        while(this->messageHandler.hasQueuedMessages()) {
            this->messageHandler.dispatch(this->messageHandler.popMessageFromQueue());
        }
    }

    void Game::run() {
        double currentTime = glfwGetTime();
        double newTime = 0;
        double updateTimeDelta = 0;
        double frameTimeDelta = 0;
        const double updateFrameTime = 1.f / this->updatesPerSecond;
        double accumulator = updateFrameTime;
        
        this->running = true;
        while(window.isOpened() && !this->aboutToClose) {
            newTime = glfwGetTime();
            
            frameTimeDelta = newTime - currentTime;
            updateTimeDelta = frameTimeDelta > 0.25 ? frameTimeDelta : 0.25;
            accumulator += updateTimeDelta;
            currentTime = newTime;
            while(accumulator >= updateFrameTime) { // Keep physics up-to-date with visuals
                accumulator -= updateFrameTime;
                
                this->processEvents(); 
                this->update(updateTimeDelta);
            }
            this->render(frameTimeDelta);
        }
        this->running = false;
    }
    
    void Game::shutdown() {
        if(this->shutdownComplete) {
           return; 
        }
        this->systemManager.stop();
        this->shutdownComplete = true;
    }
    
    bool Game::isRunning() const {
        return this->running;
    }
    
    const int Game::getUpdatesPerSecond() const {
        return this->updatesPerSecond;
    }
    
    bool Game::isAboutToClose() const {
        return this->aboutToClose;
    }
}
