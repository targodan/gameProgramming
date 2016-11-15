#include "Game.h"

#include <execinfo.h>
#include <stdlib.h>

INITIALIZE_EASYLOGGINGPP

namespace engine {
    Game::Game(int argc, char** argv, double ups) : ups(ups), entityManager(), systemManager(entityManager) {
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
        LOG(INFO) << "Engine shut down.";
    }

    void Game::initialize() {
        this->systemManager.setup();
    }
    
    void Game::run() {
        double currentTime = glfwGetTime();
        double newTime = 0.0;
        double lag = 0.0;  // Time since last update
        const double dt = 1.f / this->ups;
        double accumulator = dt;
        
        this->running = true;
        while(window.isOpened() && !this->aboutToClose) {
            newTime = glfwGetTime();
            
            lag = (newTime - currentTime) > 0.25 ? (newTime - currentTime) : 0.25;
            accumulator += lag;
            currentTime = newTime;
            while(accumulator >= dt) { // Keep physics up-to-date with visuals
                accumulator -= dt;
                
                this->processEvents(); 
                this->update();
            }
            this->render();
        }
        this->running = false;
    }
    
    void Game::shutdown() {
        this->systemManager.stop();
    }
    
    bool Game::isRunning() const {
        return this->running;
    }
    
    const int Game::getUPS() const {
        return this->ups;
    }
    
    bool Game::isAboutToClose() const {
        return this->aboutToClose;
    }
}