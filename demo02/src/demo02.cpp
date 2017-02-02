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
#include <iostream>
#include <glm/glm.hpp>
#include "IO/ButtonMapping.h"

namespace demo {
    using namespace engine::renderer::gl;
    using namespace engine::ECSCommon;
    using namespace IO;
    using glm::vec3;
    using engine::util::vector;
    using engine::Game;
    
    Demo02::Demo02(int argc, char** argv, double ups) 
        : Game(argc, argv, ups), firstMouseMovement(true) {
        this->window.setClearColor(0.f, 0.2f, 0.2f);
        
        
        
//        Material material = {std::make_shared<ShaderProgram>("../engine/defaultShader/flat_shader.vsh", 
//                                                             "../engine/defaultShader/flat_shader.fsh")};
//        
//        vector<Vertex> vertices = {Vertex(vec3{1.f, -1.f, -1.f}, vec3{1, 0, 0}, vec2{1, 0}), // 0 
//                                   Vertex(vec3{-1.f, -1.f, -1.f}, vec3{1, 0, 0}, vec2{0, 0}), // 1
//                                   Vertex(vec3{-1.f, -1.f, 1.f}, vec3{1, 0, 0}, vec2{1, 0}),  // 2
//                                   Vertex(vec3{1.f, -1.f, 1.f}, vec3{1, 0, 0}, vec2{0, 0}), // 3
//                                   Vertex(vec3{1.f, 1.f, -1.f}, vec3{1, 0, 0}, vec2{1, 1}), // 4 
//                                   Vertex(vec3{-1.f, 1.f, -1.f}, vec3{1, 0, 0}, vec2{0, 1}),  // 5
//                                   Vertex(vec3{-1.f, 1.f, 1.f}, vec3{1, 0, 0}, vec2{1, 1}), // 6
//                                   Vertex(vec3{1.f, 1.f, 1.f}, vec3{1, 0, 0}, vec2{0, 1})}; // 7
//        vector<GLuint> indices = {0,1,3, 1,2,3, 0,1,5, 0,5,4, 1,2,6, 1,6,5, 2,3,7, 2,7,6, 3,0,4, 3,4,7, 4,6,7, 4,5,6};
//        Mesh mesh = {vertices, indices};
//        mesh.loadMesh();
//       
//        this->cube.addComponent<VisualComponent>(mesh, material).addComponent<PlacementComponent>(vec3{0.f, 0.f, 0.f});
        
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
        obj.loadObject();
        
        floor.addComponent<VisualComponent>(obj).addComponent<PlacementComponent>(vec3{0.f, -1.f, 0.f});
        //auto& floorVisual = floor.getComponent(VisualComponent::getComponentTypeId()).to<VisualComponent>().getVisualObject();
        //floorVisual.loadObject();
        
        // Create bomb
        this->bomb = this->entityManager.createEntity("Bomb");
        VisualObject bombMesh = {"resources/models/bomb.obj"};
        bombMesh.loadObject();
        this->bomb.addComponent<VisualComponent>(bombMesh).addComponent<PlacementComponent>(vec3{0.f, -0.5f, 0.f});
        
        // Create second bomb
        auto bomb2 = this->entityManager.createEntity("Cube");
        VisualObject bombMesh2 = {"resources/models/bomb.obj"};
        bombMesh2.loadObject();
        bomb2.addComponent<VisualComponent>(bombMesh2).addComponent<PlacementComponent>(vec3{20.f, -0.5f, 20.f});
        
        // Create third bomb
        auto bomb3 = this->entityManager.createEntity("Cube");
        VisualObject bombMesh3 = {"resources/models/bomb.obj"};
        bombMesh3.loadObject();
        bomb3.addComponent<VisualComponent>(bombMesh3).addComponent<PlacementComponent>(vec3{5.f, -0.5f, -5.f});
       
        
        
        // Create camera/player
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{3.f, 0.f, 0.f});
        
        CameraComponent cc(vec3{-1.f, 0.f, 0.f}, vec3{0.f, 1.f, 0.f});
        cc.setProjectionMatrix(45, this->window.getAspectRatio(), 0.1f, 100.f);
        cc.setViewMatrix(pcPlayer.getPosition());
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
        
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
            // z += 0.1f;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
        
        VisualObject vo = {std::make_shared<InstanceMesh>(instanceVertices, positions), std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createFlatInstancingVertexShader(), DefaultShader::createFlatInstancingFragmentShader())))};
        vo.loadObject();
        instances.addComponent<VisualComponent>(vo).addComponent<PlacementComponent>(vec3{0.f, -0.5f, 0.f});
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
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<InputSystem>(bm);
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void Demo02::shutdown() {
        Game::shutdown();
    }
}
