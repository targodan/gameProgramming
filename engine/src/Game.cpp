#include "Game.h"

#include <execinfo.h>
#include <stdlib.h>
#include <iostream>

#include <omp.h>
#include <eigen3/Eigen/Eigen>

#include <easylogging++.h>

#include "WindowResizeMessage.h"
#include "renderer/TextRenderer.h"

namespace engine {
    Game::Game(int argc, char** argv, float ups) : updatesPerSecond(ups), messageHandler(), window(this->messageHandler, 1024,768), entityManager(this->messageHandler), systemManager(entityManager) {
        WindowResizeMessage::registerMessageType(this->messageHandler);
        engine::renderer::TextRenderer::registerForResizeMessages(this->messageHandler);
        
        engine::renderer::TextRenderer::getInstance().setWindowDimensions(this->window.getWidth(), this->window.getHeight());
        
        // Make Eigen work
        Eigen::initParallel();
        
        // Configure logger
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Filename, "engine.log");
        defaultConf.set(el::Level::Info, el::ConfigurationType::ToFile, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::ToStandardOutput, "true");
        defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s} %level : [%logger] %msg");
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        el::Loggers::setDefaultConfigurations(defaultConf, true);
        
        START_EASYLOGGINGPP(argc, argv);
        
        LOG(INFO) << "Engine started up.";
        
        // Catch uncaught exceptions and log
        std::set_terminate([] {
            // Retrieve a possibly uncaught exception.
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
        if(this->openmpThreads == -1) {
            this->openmpThreads = std::thread::hardware_concurrency();
        }
        omp_set_num_threads(this->openmpThreads);
        this->systemManager.setup();
        
        LOG(INFO) << "OpenMP running on " << this->openmpThreads << " threads.";
        LOG(INFO) << "Eigen running on " << Eigen::nbThreads() << " threads.";
        LOG(INFO) << "SystemManager running on " << this->systemManager.getNumberOfThreads() << " threads.";
    }
    
    void Game::render(float deltaTimeSeconds) {
        this->window.clear();
        this->systemManager.render(deltaTimeSeconds);
        this->window.swapBuffers();
    }
    
    void Game::update(float deltaTimeSeconds) {
        this->systemManager.update(deltaTimeSeconds);
    }
    
    void Game::processEvents() {
        glfwPollEvents();
        while(this->messageHandler.hasQueuedMessages()) {
            this->messageHandler.dispatch(this->messageHandler.popMessageFromQueue());
        }
    }

    void Game::run() {
        float gameTime = 0;
        float currentTime = glfwGetTime();
        const float updateFrameTime = 1.f / this->updatesPerSecond;
        float accumulator = updateFrameTime;
        
        this->running = true;
        while(window.isOpened() && !this->aboutToClose) {
            float newTime = glfwGetTime();
            float lagTime = newTime - currentTime;
            
            if(lagTime > 0.25) {
                lagTime = 0.25;
            }
            currentTime = newTime;
            accumulator += lagTime;
            while(accumulator >= updateFrameTime) { // Keep physics up-to-date with visuals
                this->processEvents();
                this->update(updateFrameTime);
                
                gameTime += updateFrameTime;
                accumulator -= updateFrameTime;
            }
            this->render(lagTime);
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
