#include "demo02.h"
#include "../../engine/src/ECSCommon.h"
#include "Actions.h"
#include "../../engine/src/renderer/Mesh.h"
#include "../../engine/src/renderer/InstanceMesh.h"
#include "../../engine/src/renderer/Material.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/util/vec3.h"
#include "../../engine/src/util/vector.h"
#include "../../engine/src/renderer/Vertex.h"
#include "../../engine/src/renderer/Texture.h"
#include "../../engine/src/renderer/VisualObject.h"
#include "../../engine/src/renderer/DefaultShader.h"
#include "../../engine/src/renderer/LightSource.h"
#include <iostream>
#include <glm/glm.hpp>
#include "IO/ButtonMapping.h"
#include <glm/gtx/transform.hpp>

namespace demo {
    using namespace engine::renderer::gl;
    using namespace engine::renderer;
    using namespace engine::ECSCommon;
    using namespace IO;
    using glm::vec3;
    using engine::util::vector;
    using engine::Game;
    
    Demo02::Demo02(int argc, char** argv, double ups) 
        : Game(argc, argv, ups), firstMouseMovement(true) {
        this->window.setClearColor(0.f, 0.2f, 0.2f);
        
        // Create floor
        auto floor = this->entityManager.createEntity("Floor");
        vector<Vertex> vertices = {Vertex(vec3{-100.f, 0.f, -100.f}, vec3{0.f, 0.f, 0.f}, vec2{1,0}), 
                                   Vertex(vec3{-100.f, 0.f, 100.f}, vec3{0.f, 0.f, 0.f}, vec2{0,0}), 
                                   Vertex(vec3{100.f, 0.f, -100.f}, vec3{0.f, 0.f, 0.f}, vec2{1,1}), 
                                   Vertex(vec3{100.f, 0.f, 100.f}, vec3{0.f, 0.f, 0.f}, vec2{0,1})};
        vector<GLuint> indices = {1, 0, 2,  1, 2, 3};
        Mesh mesh = {vertices, indices};
        Material material = {std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createSimpleTextureVertexShader(), DefaultShader::createSimpleTextureFragmentShader()))};
        Texture texture = {"resources/textures/container.jpg"};
        material.attachTexture(texture);
        
        VisualObject obj = {std::make_shared<Mesh>(mesh), std::make_shared<Material>(material)};
        floor.addComponent<VisualComponent>(std::make_shared<VisualObject>(obj)).addComponent<PlacementComponent>(vec3{0.f, -1.f, 0.f});
        
        // Import bomb and testing
//        VisualObject bombMesh = {"resources/models/bomb.obj"};
//        bombMesh.getMaterial().replaceTextureOfType("resources/textures/BombDiffuse.png", TextureType::DIFFUSE);
//        std::shared_ptr<VisualObject> bombMesh = std::make_shared<VisualObject>("../demoSimulation/models/bomb.blend");
//        bombMesh->getMaterial().attachTexture("../demoSimulation/textures/bomb_diffuse.png");
        std::shared_ptr<VisualObject> bombMesh = std::make_shared<VisualObject>("resources/models/models.obj");
        bombMesh->getMaterial().attachTexture("resources/textures/BombDiffuse.png");
        bombMesh->getMaterial().setShader(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createFlatVertexShader(), DefaultShader::createFlatFragmentShader())));

        bombMesh->getMesh().applyTransformation(glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)));
        auto& test = bombMesh->getMesh();
//        VisualObject bombMesh2 = *bombMesh;
//        bombMesh2.getMaterial().replaceTextureOfType("resources/textures/BombNormalMap.png", TextureType::DIFFUSE);
//        bombMesh2.getMaterial().setShader(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createSimpleTextureVertexShader(), DefaultShader::createSimpleTextureFragmentShader())));
//        VisualObject bombMesh3 = *bombMesh;
        
        bombMesh->getMaterial().enableLighting();
        bombMesh->getMaterial().setShininess(5.f);
        MaterialColor bombColor;
        bombColor.diffuse = vec3{1.f, 1.f, 1.f};
        bombColor.specular = vec3{0.f, 0.f, 0.f};
        bombColor.ambient = vec3{1.f, 1.f, 1.f};
        
        bombMesh->getMaterial().setColor(MaterialColor{});
        // Create bomb
        this->bomb = this->entityManager.createEntity("Bomb");
        this->bomb.addComponent<VisualComponent>(bombMesh).addComponent<PlacementComponent>(vec3{0.f, -0.5f, 0.f});
        
//        // Create second bomb
//        auto bomb2 = this->entityManager.createEntity("Cube");
//        bomb2.addComponent<VisualComponent>(std::make_shared<VisualObject>(bombMesh2)).addComponent<PlacementComponent>(vec3{20.f, -0.5f, 20.f});
//        
//        // Create third bomb
//        auto bomb3 = this->entityManager.createEntity("Cube2");
//        bomb3.addComponent<VisualComponent>(std::make_shared<VisualObject>(bombMesh3)).addComponent<PlacementComponent>(vec3{5.f, -0.5f, -5.f});
        
        // Create light source
        auto light = this->entityManager.createEntity("Light");
        LightSource lightSource = {vec3{0.f, 0.8f, 0.8f}, vec3{0.1f, 0.1f, 0.1f}, vec3{0.f, 0.f, 0.f}};
        light.addComponent<LightingComponent>(lightSource).addComponent<PlacementComponent>(vec3{1.f, 0.f, 1.f});
        
        // Create camera/player
        this->player = this->entityManager.createEntity("Player");
        this->player.addComponent<CameraComponent>(vec3{-1.f, 0.f, 0.f}, vec3{0.f, 1.f, 0.f}, 45, this->window.getAspectRatio(), 0.1f, 100.f).addComponent<PlacementComponent>(vec3{3.f, 0.f, 0.f});
        
        // Create instances
        auto instances = this->entityManager.createEntity("Triangles");
        vector<Vertex> instanceVertices = {Vertex{vec3{-0.1f, 0.f, -0.1f}}, Vertex{vec3{0.1f, 0.f, -0.1f}}, Vertex{vec3{0.f, 1.f, 0.f}}};
        vector<float> positions;
        
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;
        for(int i = 0; i < 100; i++) {
            x += 2.f;
            y += 2.f;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        
        VisualObject vo = {std::make_shared<InstanceMesh>(instanceVertices, positions), std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createFlatInstancingVertexShader(), DefaultShader::createFlatInstancingFragmentShader())))};
        instances.addComponent<VisualComponent>(std::make_shared<VisualObject>(vo)).addComponent<PlacementComponent>(vec3{0.f, -0.5f, 0.f});
        
        std::cout << engine::renderer::DefaultShader::createLightingFragmentShader(1, false, TextureType::DIFFUSE) << std::endl;
        // ShaderProgram prog = ShaderProgram::createShaderProgramFromSource(engine::renderer::DefaultShader::createLightingVertexShader(), engine::renderer::DefaultShader::createLightingFragmentShader(5, false, TextureType::DIFFUSE));
    }

    Demo02::~Demo02() {
        
    }
    
    void Demo02::initialize() {
        auto action1 = std::make_shared<PanCameraAction>(PanCameraAction(-2, -1, std::make_shared<Entity>(this->player), 1e-2));
        ButtonMapping bm(this->window.getWindow());
        bm.insertMapping(-2, -1, action1);
        auto action2 = std::make_shared<MoveFwdBwdAction>(MoveFwdBwdAction(-1, GLFW_KEY_W, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_W, action2);
        bm.insertMapping(-1, GLFW_KEY_S, action2, true);
        auto action3 = std::make_shared<MoveLRAction>(MoveLRAction(-1, GLFW_KEY_A, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_A, action3, true);
        bm.insertMapping(-1, GLFW_KEY_D, action3);
        auto action4 = std::make_shared<ChangeMouseMode>(ChangeMouseMode(-1, GLFW_KEY_F10, this->window.getWindow()));
        bm.insertMapping(-1, GLFW_KEY_F10, action4);
        auto action5 = std::make_shared<FlyUpDownAction>(FlyUpDownAction(-1, GLFW_KEY_Q, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_Q, action5);
        bm.insertMapping(-1, GLFW_KEY_E, action5, true);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>(this->messageHandler);
        this->systemManager.enableSystem<InputSystem>(bm);
        this->systemManager.enableSystem<RenderLoadingSystem>();
        this->systemManager.enableSystem<LightingSystem>();
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void Demo02::shutdown() {
        Game::shutdown();
    }
}
