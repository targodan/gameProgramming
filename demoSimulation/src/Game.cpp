#include "Game.h"

#include <memory>
#include <thread>

#include "engine/renderer/Mesh.h"
#include "engine/renderer/TextRenderer.h"
#include "engine/ECSCommon.h"
#include "engine/physics/Explosion.h"
#include "engine/physics/TetrahedronizedObject.h"
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
        
        auto tMesh = Tetrahedronizer::tetrahedronizeCuboid({-1, 1, 1}, {2, 0, 0}, {0, -2, 0}, {0, 0, -0.5}, 6, 6, 2, 2, 2, 0.5, density);
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
        
        VisualObject outerObject(outerMesh, outerMaterial);
        outerObject.loadObject();
        VisualObject innerObject(innerMesh, innerMaterial);
        innerObject.loadObject();
        
        this->player = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 4))
                .addComponent<CameraComponent>(glm::normalize(glm::vec3(0, 0, -1)), engine::util::vec3(0, 1, 0));
        auto& cc = this->player.getComponent<CameraComponent>();
        cc.setProjectionMatrix(120, this->window.getAspectRatio(), 0.1f, 100.f);
        
        
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
                1. / this->updatesPerSecond
            );
        defBody->freezeVertices(tMesh.getEdgeIndices());
        
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
        
//        auto explosion = std::make_shared<Explosion>(Vector3f(0, 0, 5), 1000 /* kg TNT */, SPEED_OF_SOUND_IN_AIR / 5.);
//        this->entityManager.createEntity("Force")
//                .addComponent<TimerComponent>(3)
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
        bm.insertMapping(-1, GLFW_KEY_F10, action4);
        auto action5 = std::make_shared<FlyUpDownAction>(FlyUpDownAction(-1, GLFW_KEY_Q, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_Q, action5);
        bm.insertMapping(-1, GLFW_KEY_E, action5, true);
        auto action6 = std::make_shared<BoomAction>(-2, GLFW_MOUSE_BUTTON_LEFT, *force);
        bm.insertMapping(-2, GLFW_MOUSE_BUTTON_LEFT, action6);
        
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
