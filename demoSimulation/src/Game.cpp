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

using namespace engine;
using namespace engine::renderer;
using namespace engine::physics;
using namespace engine::ECSCommon;
using namespace demo::IO;
using namespace demo;
using engine::util::vec2;
using engine::util::vec3;

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
        float density = 920; // kg / m³ rubber
        
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
        std::shared_ptr<Material> innerMaterial = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/meshColor.vsh", 
                                                         "src/meshColor.fsh"), true);
        
        auto outerObject = std::make_shared<VisualObject>(outerMesh, outerMaterial);
        outerObject->loadObject();
        auto innerObject = std::make_shared<VisualObject>(innerMesh, innerMaterial);
        innerObject->loadObject();
        
        this->player = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 1.8, 8))
                .addComponent<CameraComponent>(glm::normalize(glm::vec3(0, 0, -1)), engine::util::vec3(0, 1, 0), 120, this->window.getAspectRatio(), 0.1f, 100.f);
        
        
        this->tetrahedron = this->entityManager.createEntity("Inner")
                .addComponent<VisualComponent>(innerObject)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        this->tetrahedron = this->entityManager.createEntity("Outer")
                .addComponent<VisualComponent>(outerObject)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        
//        TetrahedronizedObject tMesh(
//                ObjectProperties::verticesToFlatVector(tetrahedronMesh->getVertices()),
//                {tetrahedronMesh},
//                {{std::make_pair(0, 0)}, {std::make_pair(0, 1)}, {std::make_pair(0, 2)}, {std::make_pair(0, 3)}},
//                ObjectProperties(
//                        ObjectProperties::verticesToFlatVector(tetrahedronMesh->getVertices()),
//                        {0, 1, 2, 3},
//                        Vector4f(1, 1, 1, 1),
//                        Vector4f(1, 1, 1, 1)),
//                {0, 1, 2, 3});
        
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
        defBody->freezeVertices(tMesh.getEdgeIndices());
        
        this->openmpThreads = std::thread::hardware_concurrency() * 4;
        
        // Deformable body created => restPosition copied
        // Let's now pull on a vertex.
//        defBody->getCurrentPosition()[1] += 0.001;
//        defBody->getCurrentPosition()[2] += 0.025;
//        tMesh.getMesh(0).getVertices()[0].position.y += 0.001;
//        tMesh.getMesh(0).getVertices()[0].position.z += 0.025;
//        tMesh.getMesh(0).loadMesh();
        
//        VisualObject tetraObject(tetrahedronMesh, innerMaterial);
//        tetraObject.loadObject();
        this->tetrahedron = this->entityManager.createEntity("DefBody")
                .addComponent<DeformableBodyComponent>(defBody);
        
        auto force = std::make_shared<OneShotForce>();
        this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(0)
                .addComponent<ForceComponent>(force);
        
//        this->entityManager.createEntity("Gravity")
//                .addComponent<TimerComponent>(0)
//                .addComponent<ForceComponent>(std::make_shared<GravitationalForce>(GRAVITY_G_TO_M_PER_SS(0.75)));
//        
//        auto explosion = std::make_shared<Explosion>(Vector3f(0, 0, 5), 10 /* kg TNT */, SPEED_OF_SOUND_IN_AIR / 10.);
//        this->entityManager.createEntity("Force")
//                .addComponent<TimerComponent>(3)
//                .addComponent<ForceComponent>(explosion);
        
//        this->systemManager.enableSystem<CustomUpdateSystem>("boom", [](EntityManager& em, float dT) {
//            for(auto it = em.begin({TimerComponent::getComponentTypeId(), ForceComponent::getComponentTypeId()}); it != em.end(); ++it) {
//                if(it->to<TimerComponent>().getTime() > 0) {
//                    LOG(INFO) << "Boom";
//                }
//            }
//        });
        
        LOG(INFO) << "Test";
        auto bombVO = std::make_shared<VisualObject>("models/bomb.obj");
        LOG(INFO) << "Another Test";
        bombVO->getMesh().applyTransformation(glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)));
        bombVO->loadObject();
        this->entityManager.createEntity("Bomb")
                .addComponent<VisualComponent>(bombVO)
                .addComponent<PlacementComponent>(glm::vec3{0.f, 0.3, 3});
        
        float floorRadius = 1e4;
        float floorTexRepeat = 4e4;
        auto floorMesh = std::make_shared<Mesh>(engine::util::vector<Vertex>({
                Vertex({-floorRadius, 0, -floorRadius}, {0, floorRadius, 0}, {0, floorTexRepeat}),
                Vertex({-floorRadius, 0,  floorRadius}, {0, floorRadius, 0}, {0, 0}),
                Vertex({ floorRadius, 0,  floorRadius}, {0, floorRadius, 0}, {floorTexRepeat, 0}),
                Vertex({ floorRadius, 0, -floorRadius}, {0, floorRadius, 0}, {floorTexRepeat, floorTexRepeat}),
            }), engine::util::vector<GLuint>({
                0, 1, 2,
                0, 2, 3
            }));
            LOG(INFO) << "TEST!";
        Texture floorTex("textures/floor_diffuse.png");
        LOG(INFO) << "TEST!!";
        auto floorMat = std::make_shared<Material>(
            std::make_shared<ShaderProgram>(
                ShaderProgram::createShaderProgramFromSource(
                    DefaultShader::createSimpleTextureVertexShader(),
                    DefaultShader::createSimpleTextureFragmentShader()
                )
            )
        );
        floorMat->attachTexture(floorTex);
        auto floorVO = std::make_shared<VisualObject>(floorMesh, floorMat);
        floorVO->loadObject();
        this->entityManager.createEntity("Floor")
                .addComponent<VisualComponent>(floorVO);
        
        auto PatSys = this->entityManager.createEntity("ParticleSystem");
        engine::util::vector<Vertex> instanceVertices = { Vertex{engine::util::vec3{-0.2f, -0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 0}}, 
                                            Vertex{engine::util::vec3{0.2f, -0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 0}},
                                            Vertex{engine::util::vec3{-0.2f, 0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 1}},
                                            Vertex{engine::util::vec3{0.2f, 0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 0}},
                                            Vertex{engine::util::vec3{-0.2f, 0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{1, 1}},
                                            Vertex{engine::util::vec3{0.2f, -0.2f, 0.f}, engine::util::vec3{0.f, 0.f, 0.f}, engine::util::vec2{0, 1}}};
        engine::util::vector<float> positions(300, 0.0f);
        auto iMesh = std::make_shared<InstanceMesh>(instanceVertices, positions);
        auto iMat = std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createTextureInstancingVertexShader(), DefaultShader::createTextureInstancingFragmentShader())));
        Texture iTex = {"textures/NeGeo.png"};
        iMat->attachTexture(iTex);
        VisualObject vo = {iMesh, iMat};
        vo.loadObject();
        
        ParticleSystem pats(3, 0.2, iMesh, ParticleForce::getForceOnVertices(300));
        std::shared_ptr<ParticleSystem> patsptr = std::make_shared<ParticleSystem>(pats);
        
        PatSys.addComponent<VisualComponent>(std::make_shared<VisualObject>(vo)).addComponent<PlacementComponent>(engine::util::vec3{0.f, -0.5f, 0.f}).addComponent<ParticleSystemComponent>(patsptr);
        
        
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
        auto action6 = std::make_shared<BoomAction>(-2, GLFW_MOUSE_BUTTON_LEFT, *force);
        bm.insertMapping(-2, GLFW_MOUSE_BUTTON_LEFT, action6);
        
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
