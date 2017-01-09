#include "Game.h"
#include "engine/ECSCommon.h"
#include "LevelLoader.h"

namespace explosionBoy {
    void Game::initialize() {
        this->systemManager.enableSystem<engine::ECSCommon::PlacementSystem>();
        this->systemManager.enableSystem<engine::ECSCommon::RenderSystem>();
        
        this->currentLevel = LevelLoader::loadLevel("./resources/levels/example.level", this->entityManager);
        
        engine::Game::initialize();
    }
}