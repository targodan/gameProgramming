#include "Game.h"

#include "engine/renderer/Mesh.h"
#include "engine/ECSCommon.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::ECSCommon;

namespace demoSimulation {
    void Game::initialize() {
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>();
                
        Vertex frontBottom({0, 0, 0.5});
        Vertex backLeft({-0.5, 0, -0.5});
        Vertex backRight(0.5, 0, -0.5);
        Vertex up(0, 1, 0);
        Mesh tetrahedronMesh({frontBottom, backRight, backLeft, up}, {0, 1, 3, 2, 0, 3, 1, 2, 3});
        tetrahedronMesh.loadMesh();
        
        Material material(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"));
        
        this->tetrahedron = this->entityManager.createEntity("Tetrahedron")
                .addComponent<VisualComponent>(tetrahedronMesh, material)
                .addComponent<PlacementComponent>();
        
        engine::Game::initialize();
    }
}
