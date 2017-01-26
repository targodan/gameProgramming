#include "Game.h"

#include <memory>
#include <thread>

#include "engine/renderer/Mesh.h"
#include "engine/renderer/TextRenderer.h"
#include "engine/ECSCommon.h"
#include "engine/physics/Explosion.h"
#include "engine/physics/TetrahedronizedMesh.h"
#include "engine/physics/Tetrahedronizer.h"
#include "OneShotForce.h"
#include "Actions.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::physics;
using namespace engine::ECSCommon;
using namespace demo::IO;

namespace demoSimulation {
    void Game::initialize() {
        this->window.setClearColor(0.1f, 0.f, 0.1f);
        
        LOG(INFO) << "Window dimensions: " << this->window.getWidth() << "x" << this->window.getHeight();
        
        auto& fontfamiliy = FontRegistry::registerFontFamily(
            "DejaVuSans",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-BoldOblique.ttf"
        );

        Vertex frontBottom( {0, 0.5, 0.5},        {0, 1, 0});
        Vertex backLeft(    {-0.5, 0, 0},    {0, 0, 1});
        Vertex backRight(   {0.5,  0, 0},     {1, 0, 0});
        Vertex backBottom(  {0, 0.5, -0.5},       {1, 0, 1});
        Vertex up(          {0, 1, 0},          {1, 1, 0});
//        Mesh tetrahedronMesh({frontBottom, backRight, backLeft, up},
//                {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2, 1, 3, 2},
//                DataUsagePattern::DYNAMIC_DRAW);
//        std::shared_ptr<Mesh> tetrahedronMesh = std::shared_ptr<Mesh>(new Mesh({frontBottom, backRight, backLeft, up, backBottom},
//                {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2, 1, 3, 2, 1, 4, 3, 4, 2, 3, 1, 2, 4},
//                DataUsagePattern::DYNAMIC_DRAW));
        
        auto tMesh = Tetrahedronizer::tetrahedronizeCuboid({-1, 1, 1}, {2, 0, 0}, {0, -2, 0}, {0, 0, -2}, 4, 4, 4);
        std::shared_ptr<Mesh> tetrahedronMesh = tMesh.getMeshPtr();
        tetrahedronMesh->loadMesh();
        
        std::shared_ptr<Material> material = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"), true);
        
        this->player = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0.5, 2))
                .addComponent<CameraComponent>(engine::util::vec3(0, 0, -1), engine::util::vec3(0, 1, 0));
        auto& cc = this->player.getComponent<CameraComponent>();
        cc.setProjectionMatrix(120, this->window.getAspectRatio(), 0.1f, 100.f);
        cc.setViewMatrix(engine::util::vec3(0, 0.5, 2));
        
        
        this->tetrahedron = this->entityManager.createEntity("Tetrahedron")
                .addComponent<VisualComponent>(tetrahedronMesh, *material)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        
        auto& vc = this->tetrahedron.getComponent<VisualComponent>();
        
//        TetrahedronizedMesh tMesh(vc.getMesh(), {0, 1, 2, 3});
//        TetrahedronizedMesh tMesh(tetrahedronMesh, {0, 1, 2, 3, 1, 2, 3, 4});
        
        float volume = tMesh.calculateVolume();
        float area = 5;
//        float density = 7850; // kg / m³ metal
        float density = 920; // kg / m³ rubber
        float mass = volume * density / 5;
        
        auto properties = 
                engine::physics::ObjectProperties::uniformTetrahedronDistribution(
                            engine::physics::ObjectProperties::verticesToFlatVector(vc.getMesh().getVertices()), engine::util::vector<size_t>({0, 1, 2, 3})
                        )
                        .uniformDensity(volume, density)
                        .uniformAreaDistribution(area);
        
        auto defBody = std::make_shared<engine::physics::DeformableBody>(
                tMesh,
                properties,
                mass,
                5, // dampening
                0.05e9, // youngs modulus rubber
                0.4999, // poissons ratio rubber
//                200e9, // youngs modulus metal
//                0.27, // poissons ratio metal
                1. / this->updatesPerSecond
            );
        
        auto defBodyEntity = this->entityManager.createEntity("DeformableBody")
                .addComponent<DeformableBodyComponent>(defBody);
        
        auto force = std::make_shared<OneShotForce>();
        this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(5)
                .addComponent<ForceComponent>(force);
        
//        auto explosion = std::make_shared<Explosion>(Vector3f(-5.5, 0.5, 0), 100 /* kg TNT */, SPEED_OF_SOUND_IN_AIR / 5.);
//        this->entityManager.createEntity("Force")
//                .addComponent<TimerComponent>(5)
//                .addComponent<ForceComponent>(explosion);
        
        RichText testText(fontfamiliy, FontType::Regular, 80, Color::RED);
        testText << u8"Test öä§∑. "
                << RichText::fontType(FontType::Bold) << RichText::color(Color(1, 1, 0, 1)) << u8"Bold "
                << RichText::fontType(FontType::Italic) << RichText::color(Color::BLUE) << u8"Italic "
                << RichText::fontType(FontType::BoldItalic) << RichText::color(Color::GREEN) << u8"BoldItalic ";
        
        LOG(INFO) << testText.getPlainText_utf8();
        
//        this->entityManager.createEntity("testtext")
//                .addComponent<TextComponent>(testText, 50, 100);
        
//        this->systemManager.enableSystem<CustomUpdateSystem>("boom", [](EntityManager& em, float dT) {
//            for(auto it = em.begin({TimerComponent::getComponentTypeId(), ForceComponent::getComponentTypeId()}); it != em.end(); ++it) {
//                if(it->to<TimerComponent>().getTime() > 0) {
//                    LOG(INFO) << "Boom";
//                }
//            }
//        });
        
        auto action1 = std::make_shared<PanCameraAction>(PanCameraAction(-2, -1, std::make_shared<Entity>(this->player), 2e-2));
        ButtonMapping bm(this->window.getWindow());
        bm.insertMapping(-2, -1, action1);
        auto action2 = std::make_shared<MoveFwdBwdAction>(MoveFwdBwdAction(-1, GLFW_KEY_W, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_W, action2);
        bm.insertMapping(-1, GLFW_KEY_S, action2, true);
        auto action3 = std::make_shared<MoveLRAction>(MoveLRAction(-1, GLFW_KEY_A, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_A, action3, true);
        bm.insertMapping(-1, GLFW_KEY_D, action3);
        auto action4 = std::make_shared<ChangeMouseMode>(ChangeMouseMode(-1, GLFW_KEY_F10, this->window.getWindow()));
        bm.insertMapping(-1, GLFW_KEY_ESCAPE, action4);
        auto action5 = std::make_shared<FlyUpDownAction>(FlyUpDownAction(-1, GLFW_KEY_Q, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_Q, action5);
        bm.insertMapping(-1, GLFW_KEY_E, action5, true);
        
        this->systemManager.enableSystem<InputSystem>(bm);
        
        this->systemManager.enableSystem<PerformanceMetricsSystem>(&this->entityManager);
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<DeformableBodySystem>();
        this->systemManager.enableSystem<TimerSystem>();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        engine::Game::initialize();
    }
    
    void Game::render(float deltaTimeSeconds) {
        engine::Game::render(deltaTimeSeconds);
    }
}