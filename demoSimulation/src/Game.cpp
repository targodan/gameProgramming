#include "Game.h"

#include "engine/renderer/Mesh.h"
#include "engine/ECSCommon.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::ECSCommon;

namespace demoSimulation {
    void Game::initialize() {
        this->window.setClearColor(0.f, 0.2f, 0.2f);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<RenderSystem>();
                
        Vertex frontBottom({0, 0, 0.5});
        Vertex backLeft({-0.5, 0, -0.5});
        Vertex backRight({0.5, 0, -0.5});
        Vertex up({0, 1, 0});
        Mesh tetrahedronMesh({frontBottom, backRight, backLeft, up}, {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2});
        tetrahedronMesh.loadMesh();
        
        std::shared_ptr<Material> material = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"));
        
        this->camera = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0.5, 2))
                .addComponent<CameraComponent>(engine::util::vec3(0, 0, -1), engine::util::vec3(0, 1, 0));
        auto& cc = this->camera.getComponent<CameraComponent>();
        cc.setProjectionMatrix(120, this->window.getAspectRatio(), 0.1, 10);
        
        
        this->tetrahedron = this->entityManager.createEntity("Tetrahedron")
                .addComponent<VisualComponent>(tetrahedronMesh, *material)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        
        engine::Game::initialize();
    }
}
