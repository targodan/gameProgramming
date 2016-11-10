#include "Game.h"

#include <execinfo.h>
#include <stdlib.h>

INITIALIZE_EASYLOGGINGPP

namespace engine {
    Game::Game() : entityManager(), systemManager(entityManager) {
        // TODO: Configure logger. Ideally to log both to a file and to stderr.
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
            std::abort();
        });
    }
    
    Game::~Game() {}

    void Game::initialize() {
        this->systemManager.setup();
    }
    
    void Game::run() {
        // TODO: main loop
    }
    
    void Game::shutdown() {
        this->systemManager.stop();
    }
}