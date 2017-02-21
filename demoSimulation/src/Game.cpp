#include "Game.h"

#include <memory>
#include <thread>
#include <glm/gtx/transform.hpp>

#include "engine/renderer/Mesh.h"
#include "engine/renderer/TextRenderer.h"
#include "engine/renderer/Skybox.h"
#include "engine/ECSCommon.h"
#include "engine/physics/Explosion.h"
#include "engine/physics/GravitationalForce.h"
#include "engine/physics/TetrahedronizedObject.h"
#include "engine/physics/Tetrahedronizer.h"
#include "engine/util/vec3.h"
#include "engine/util/vec2.h"
#include "ParticleForce.h"
#include "OneShotForce.h"
#include "Actions.h"
#include "renderer/DefaultShader.h"
#include "engine/renderer/LightSource.h"
#include "util/vec3.h"

#include "freezer.h"
#include "Actions/FreezeAction.h"
#include "Actions/ResetAction.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::physics;
using namespace engine::ECSCommon;
using namespace demo;
using namespace demo::IO;
using engine::util::vec2;
using engine::util::vec3;
using engine::util::vector;

/*
 * --- Button Binding ---
 * 
 * WASD, QE : Movement
 * ESC/F10  : Toggle mouse mode
 * 
 * R        : Reset Simulation
 * 
 * LeftMouse: Push wall, start particles and hide bomb
 * F        : Trigger explosion force
 * G        : Toggle gravity
 * H        : Toggle freeze left part of wall
 * J        : Toggle freeze right part of wall
 * K        : Toggle freeze top part of wall
 * L        : Toggle freeze bottom part of wall
 */

namespace demoSimulation {
    void Game::initialize() {
        this->window.setClearColor(0.1f, 0.f, 0.1f);
        
        LOG(INFO) << "Window dimensions: " << this->window.getWidth() << "x" << this->window.getHeight();

        FontRegistry::registerFontFamily(
            "DejaVuSans",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-BoldOblique.ttf"
        );
        
        this->statusDisplay = new StatusDisplay(this->entityManager);

        Vertex frontBottom( {0, 0.05, 0.05},        {0, 1, 0});
        Vertex backLeft(    {-0.05, 0, 0},    {0, 0, 1});
        Vertex backRight(   {0.05,  0, 0},     {1, 0, 0});
        Vertex backBottom(  {0, 0.05, -0.05},       {1, 0, 1});
        Vertex up(          {0, 0.1, 0},          {1, 1, 0});
//        std::shared_ptr<Mesh> tetrahedronMesh = std::shared_ptr<Mesh>(new Mesh({frontBottom, backRight, backLeft, up},
//                {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2, 1, 3, 2},
//                DataUsagePattern::DYNAMIC_DRAW));
//        std::shared_ptr<Mesh> tetrahedronMesh = std::shared_ptr<Mesh>(new Mesh({frontBottom, backRight, backLeft, up, backBottom},
//                {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2, 1, 3, 2, 1, 4, 3, 4, 2, 3, 1, 2, 4},
//                DataUsagePattern::DYNAMIC_DRAW));

//        float density = 7850; // kg / m³ metal
//        float density = 900; // kg / m³ rubber
        float density = 400; // looks cooler

        auto tMesh = Tetrahedronizer::tetrahedronizeCuboid({-2, 4, 2}, {4, 0, 0}, {0, -4, 0}, {0, 0, -0.5}, 16, 16, 1, 8, 8, 2, density);
        std::shared_ptr<Mesh> outerMesh = tMesh.getMeshPtr(0);
        std::shared_ptr<Mesh> innerMesh = tMesh.getMeshPtr(1);

        float volume = tMesh.calculateVolume();
        LOG(INFO) << "Volume: " << volume << " m³";
        LOG(INFO) << "Surface (via surfaces): " << tMesh.getProperties().surfaceAreaPerVertex.sum() << " m²";
        LOG(INFO) << "Surface (by hand): " << 2 * 2 * 2 + 4 * 2 * 0.5 << " m²";
        LOG(INFO) << "Total mass (via masses): " << tMesh.getProperties().massPerVertex.sum() << " kg";
        LOG(INFO) << "Total mass (via density): " << volume * density << " kg";

        Texture outerTexture("textures/wall.png");

        std::shared_ptr<Material> outerMaterial = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/textured.vsh",
                                                         "src/textured.fsh"));
        outerMaterial->attachTexture(outerTexture);
        outerMaterial->setSpecular(util::vec3{0, 0, 0});
        outerMaterial->setShininess(1e3);
        outerMaterial->enableLighting();

        std::shared_ptr<Material> innerMaterial = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/meshColor.vsh",
                                                         "src/meshColor.fsh"), true);

        auto outerObject = std::make_shared<VisualObject>(outerMesh, outerMaterial);
        auto innerObject = std::make_shared<VisualObject>(innerMesh, innerMaterial);

        this->player = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 1.8, 8))
                .addComponent<CameraComponent>(glm::normalize(glm::vec3(0, 0, -1)), engine::util::vec3(0, 1, 0), 90, this->window.getAspectRatio(), 0.01f, 50.f);


//        this->tetrahedron = this->entityManager.createEntity("Inner")
//                .addComponent<VisualComponent>(innerObject)
//                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        this->tetrahedron = this->entityManager.createEntity("Outer")
                .addComponent<VisualComponent>(outerObject)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));

        auto defBody = std::make_shared<engine::physics::DeformableBody>(
                tMesh,
                1, // dampening
                0.01e6, // youngs modulus rubber
                0.49, // poissons ratio rubber
//                200e9, // youngs modulus metal
//                0.27, // poissons ratio metal
                1. / this->updatesPerSecond,
                0
            );
        // defBody->freezeVertices(tMesh.getEdgeIndices());
        this->statusDisplay->setLeftFrozen(false);
        this->statusDisplay->setRightFrozen(false);
        this->statusDisplay->setTopFrozen(false);
        this->statusDisplay->setBottomFrozen(false);
        
        this->openmpThreads = std::thread::hardware_concurrency() * 4;

        this->tetrahedron = this->entityManager.createEntity("DefBody")
                .addComponent<DeformableBodyComponent>(defBody);

        auto force = std::make_shared<OneShotForce>();
        this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(0)
                .addComponent<ForceComponent>(force);

        auto gravity = std::make_shared<GravitationalForce>(GRAVITY_G_TO_M_PER_SS(0.6));
        gravity->disable();
        this->statusDisplay->setGravity(false);
        auto gravEnt = this->entityManager.createEntity("Gravity")
                .addComponent<TimerComponent>(0)
                .addComponent<ForceComponent>(gravity);
//
        auto explosion = std::make_shared<Explosion>(Vector3f(0, 0.3, 5), 2 /* kg TNT */, SPEED_OF_SOUND_IN_AIR / 10.);
        auto expEnt = this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(9999999999)
                .addComponent<ForceComponent>(explosion);

//        this->systemManager.enableSystem<CustomUpdateSystem>("boom", [](EntityManager& em, float dT) {
//            for(auto it = em.begin({TimerComponent::getComponentTypeId(), ForceComponent::getComponentTypeId()}); it != em.end(); ++it) {
//                if(it->to<TimerComponent>().getTime() > 0) {
//                    LOG(INFO) << "Boom";
//                }
//            }
//        });

        VisualObject bombVO = {"models/bomb.obj"};
        bombVO.getMaterial().attachTexture("textures/bomb_diffuse.png", TextureType::DIFFUSE);
        bombVO.getMaterial().attachTexture("textures/bomb_specular.png", TextureType::SPECULAR);
        bombVO.getMaterial().setShader(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createSimpleTextureVertexShader(), DefaultShader::createSimpleTextureFragmentShader())));
        // bombVO->getMesh().applyTransformation(glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)));
        bombVO.getMaterial().enableLighting();
        bombVO.getMaterial().setShininess(64.f);
        bombVO.getMaterial().setSpecular(util::vec3(0.6, 0.6, 0.6));
         // bombVO->loadObject();
        auto bombEnt = this->entityManager.createEntity("Bomb")
                .addComponent<VisualComponent>(std::make_shared<VisualObject>(bombVO))
                .addComponent<PlacementComponent>(glm::vec3{0.f, -10.3, 3});

        float floorRadius = 1e4;
        float floorTexRepeat = 4e4;
        auto floorMesh = std::make_shared<Mesh>(engine::util::vector<Vertex>({
                Vertex({-floorRadius, 0, -floorRadius}, {0, 1, 0}, {0, floorTexRepeat}),
                Vertex({-floorRadius, 0,  floorRadius}, {0, 1, 0}, {0, 0}),
                Vertex({ floorRadius, 0,  floorRadius}, {0, 1, 0}, {floorTexRepeat, 0}),
                Vertex({ floorRadius, 0, -floorRadius}, {0, 1, 0}, {floorTexRepeat, floorTexRepeat}),
            }), engine::util::vector<GLuint>({
                0, 1, 2,
                0, 2, 3
            }));

        auto floorMat = std::make_shared<Material>(
            std::make_shared<ShaderProgram>(
                ShaderProgram::createShaderProgramFromSource(
                    DefaultShader::createSimpleTextureVertexShader(),
                    DefaultShader::createSimpleTextureFragmentShader()
                )
            )
        );
        floorMat->attachTexture("textures/floor_diffuse.png");
        floorMat->setSpecular(util::vec3(0, 0, 0));
        floorMat->setShininess(0);
        floorMat->enableLighting();
        auto floorVO = std::make_shared<VisualObject>(floorMesh, floorMat);

        this->entityManager.createEntity("Floor")
                .addComponent<VisualComponent>(floorVO);

        auto light = this->entityManager.createEntity("Light");
        auto lightSource = std::make_shared<LightSource>(util::vec3{0.81, 0.805, 0.8}, util::vec3{0.2, 0.2, 0.2});
        lightSource->setAttenuation(1e-4, 1e-4);
        light.addComponent<LightingComponent>(lightSource)
            .addComponent<PlacementComponent>(util::vec3{5.f, 8, 5.f});

//        auto light2 = this->entityManager.createEntity("Light2");
//        LightSource lightSource2 = {util::vec3{0.8f, 0.8f, 0.8f}};
//        lightSource2.setAttenuation(0.1, 0.1);
//        light2.addComponent<LightingComponent>(std::make_shared<LightSource>(lightSource2)).addComponent<PlacementComponent>(util::vec3{-5.f, 0.f, -5.f});

        auto PatSys = this->entityManager.createEntity("ParticleSystem");
        int numPats = 1000;
        engine::util::vector<Vertex> instanceVertices = { Vertex{engine::util::vec3{-0.05f, -0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 0}},
                                            Vertex{engine::util::vec3{0.05f, -0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 0}},
                                            Vertex{engine::util::vec3{-0.05f, 0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 1}},
                                            Vertex{engine::util::vec3{0.05f, 0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 0}},
                                            Vertex{engine::util::vec3{-0.05f, 0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 1}},
                                            Vertex{engine::util::vec3{0.05f, -0.05f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 1}}};
        engine::util::vector<float> positions(3*numPats, 0.0f);
        std::shared_ptr<InstanceMesh> iMesh = std::make_shared<InstanceMesh>(instanceVertices, positions);
        std::shared_ptr<VisualObject> vo = std::make_shared<VisualObject>(iMesh, std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createTextureInstancingVertexShader(), DefaultShader::createTextureInstancingFragmentShader()))));
        Texture tex("textures/particle.png", ImageFormat::RGBA, ImageFormat::RGBA, TextureDimension::TEXTURE_2D, TextureType::DIFFUSE);
        vo->getMaterial().attachTexture(tex);

        std::shared_ptr<ParticleSystem> patsptr = std::make_shared<ParticleSystem>(1e-2, 1e-6, 0, iMesh, ParticleForce::getForceOnVertices(numPats, 4, 8));

        // PatSys.addComponent<VisualComponent>(vo).addComponent<PlacementComponent>(engine::util::vec3{0.f, 0.3, 3}).addComponent<ParticleSystemComponent>(patsptr);

//        auto instances = this->entityManager.createEntity("Triangles");
//        engine::util::vector<Vertex> instanceVertices = {Vertex{engine::util::vec3{-0.1f, 0.f, -0.1f}}, Vertex{engine::util::vec3{0.1f, 0.f, -0.1f}}, Vertex{engine::util::vec3{0.f, 1.f, 0.f}}};
//        engine::util::vector<float> positions;
//
//        float x = 0.f;
//        float y = 0.f;
//        float z = 0.f;
//        for(int i = 0; i < 100; i++) {
//            x += 2.f;
//            y += 2.f;
//            positions.push_back(x);
//            positions.push_back(y);
//            positions.push_back(z);
//        }
//
//        VisualObject vo = {std::make_shared<InstanceMesh>(instanceVertices, positions), std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createFlatInstancingVertexShader(), DefaultShader::createFlatInstancingFragmentShader())))};
//        instances.addComponent<VisualComponent>(std::make_shared<VisualObject>(vo)).addComponent<PlacementComponent>(engine::util::vec3{0.f, -0.5f, 0.f});

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
        bm.insertMapping(-1, GLFW_KEY_F10, action4);
        auto action5 = std::make_shared<FlyUpDownAction>(FlyUpDownAction(-1, GLFW_KEY_Q, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_Q, action5);
        bm.insertMapping(-1, GLFW_KEY_E, action5, true);
        auto action6 = std::make_shared<BoomAction>(-2, GLFW_MOUSE_BUTTON_LEFT, *force, patsptr, bombEnt.getComponent<PlacementComponent>());
        bm.insertMapping(-2, GLFW_MOUSE_BUTTON_LEFT, action6);
        bm.insertMapping(-1, GLFW_KEY_F, std::make_shared<ResetTimerAction>(-1, GLFW_KEY_J, expEnt.getComponent<TimerComponent>()));
        bm.insertMapping(-1, GLFW_KEY_G, std::make_shared<ToggleForceAction>(-1, GLFW_KEY_G, gravEnt.getComponent<ForceComponent>().getForce(), this->statusDisplay));
        
        bm.insertMapping(-1, GLFW_KEY_R, std::make_shared<ResetAction>(-1, GLFW_KEY_R, *defBody));
        bm.insertMapping(-1, GLFW_KEY_H, std::make_shared<FreezeAction>(-1, GLFW_KEY_H, *defBody, tMesh, this->statusDisplay));
        bm.insertMapping(-1, GLFW_KEY_J, std::make_shared<FreezeAction>(-1, GLFW_KEY_J, *defBody, tMesh, this->statusDisplay));
        bm.insertMapping(-1, GLFW_KEY_K, std::make_shared<FreezeAction>(-1, GLFW_KEY_K, *defBody, tMesh, this->statusDisplay));
        bm.insertMapping(-1, GLFW_KEY_L, std::make_shared<FreezeAction>(-1, GLFW_KEY_L, *defBody, tMesh, this->statusDisplay));

        Texture skyTexture("textures/skybox_small.png");
        auto skybox = std::make_shared<Skybox>(skyTexture, EnvironmentTextureType::EQUIRECTANGULAR);
        skybox->loadObject();
        this->tetrahedron = this->entityManager.createEntity("Skybox")
                .addComponent<VisualComponent>(skybox);

        this->systemManager.enableSystem<InputSystem>(bm);

        this->systemManager.enableSystem<PerformanceMetricsSystem>(this->entityManager);
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>(this->messageHandler);
        this->systemManager.enableSystem<DeformableBodySystem>();
        this->systemManager.enableSystem<TimerSystem>();
        this->systemManager.enableSystem<LightingSystem>();
        this->systemManager.enableSystem<ParticleSystemSystem>();
        this->systemManager.enableSystem<ForceTimerSystem>();

        this->entityManager.sort(VisualComponent::getComponentTypeId(), [](std::shared_ptr<Component> c1, std::shared_ptr<Component> c2) {
            return c1->to<VisualComponent>().getVisualObject().getRenderPriority() < c2->to<VisualComponent>().getVisualObject().getRenderPriority();
        });

        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        engine::Game::initialize();
    }

    void Game::render(float deltaTimeSeconds) {
        engine::Game::render(deltaTimeSeconds);
    }
}
