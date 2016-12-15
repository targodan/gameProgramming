#include "Game.h"
#include "engine/ECSCommon/PlacementSystem.h"

namespace explosionBoy {
    void Game::initialize() {
        this->systemManager.enableSystem<engine::ECSCommon::PlacementSystem>();
        engine::Game::initialize();
    }
}