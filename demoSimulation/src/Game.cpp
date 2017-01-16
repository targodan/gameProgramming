#include "Game.h"

#include <memory>

#include "engine/renderer/Mesh.h"
#include "engine/ECSCommon.h"
#include "OneShotForce.h"
#include "Actions.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::ECSCommon;
using namespace demo::IO;

namespace demoSimulation {
    void Game::initialize() {
        this->window.setClearColor(0.1f, 0.f, 0.1f);

        Vertex frontBottom({0, 0, 0.5}, {0, 1, 0});
        Vertex backLeft({-0.5, 0, -0.5}, {0, 0, 1});
        Vertex backRight({0.5, 0, -0.5}, {1, 0, 0});
        Vertex up({0, 1, 0}, {1, 1, 0});
        Mesh tetrahedronMesh({frontBottom, backRight, backLeft, up}, {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2});
        tetrahedronMesh.loadMesh();
        
        std::shared_ptr<Material> material = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"), true);
        
        this->camera = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0.5, 2))
                .addComponent<CameraComponent>(engine::util::vec3(0, 0, -1), engine::util::vec3(0, 1, 0));
        auto& cc = this->camera.getComponent<CameraComponent>();
        cc.setProjectionMatrix(120, this->window.getAspectRatio(), 0.1, 10);
        
        LOG(DEBUG) << "CameraDirection: " << cc.getDirection();
        LOG(DEBUG) << "CameraUp: " << cc.getUp();
        
        
        this->tetrahedron = this->entityManager.createEntity("Tetrahedron")
                .addComponent<VisualComponent>(tetrahedronMesh, *material)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        
        auto& vc = this->tetrahedron.getComponent<VisualComponent>();
        
        float volume = 0.1;
        float area = 5;
        float density = 920; // kg / m³
        float mass = volume * density;
        
        auto properties = 
                engine::physics::ObjectProperties::uniformTetrahedronDistribution(
                            engine::physics::ObjectProperties::verticesToFlatVector(vc.getMesh().getVertices()), engine::util::vector<size_t>({0, 1, 2, 3})
                        )
                        .uniformDensity(volume, density)
                        .uniformAreaDistribution(area);
        
        auto defBody = std::make_shared<engine::physics::DeformableBody>(
                vc.getMesh(),
                properties,
                mass,
                0.1,
                0.05e9,
                0.4999,
                this->updatesPerSecond
            );
        
        auto defBodyEntity = this->entityManager.createEntity("DeformableBody")
                .addComponent<DeformableBodyComponent>(defBody);
        
        auto force = std::make_shared<OneShotForce>();
        this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(5)
                .addComponent<ForceComponent>(force);
        
        auto action1 = std::make_shared<PanCameraAction>(-2, -1, std::make_shared<Entity>(this->camera));
        ButtonMapping bm(this->window.getWindow());
        bm.insertMapping(-2, -1, action1);
        auto action2 = std::make_shared<MoveFwdBwdAction>(-1, GLFW_KEY_W, std::make_shared<Entity>(this->camera));
        bm.insertMapping(-1, GLFW_KEY_W, action2);
        bm.insertMapping(-1, GLFW_KEY_S, action2, true);
        auto action3 = std::make_shared<MoveLRAction>(-1, GLFW_KEY_A, std::make_shared<Entity>(this->camera));
        bm.insertMapping(-1, GLFW_KEY_A, action3, true);
        bm.insertMapping(-1, GLFW_KEY_D, action3);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<DeformableBodySystem>();
        this->systemManager.enableSystem<TimerSystem>();
        this->systemManager.enableSystem<InputSystem>(bm);
        
        engine::Game::initialize();
    }
}
