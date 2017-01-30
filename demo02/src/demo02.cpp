#include "demo02.h"
#include "../../engine/src/ECSCommon.h"
#include "Actions.h"
#include "../../engine/src/renderer/Mesh.h"
#include "../../engine/src/renderer/Material.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/util/vec3.h"
#include "../../engine/src/util/vector.h"
#include "../../engine/src/renderer/Vertex.h"
#include "../../engine/src/renderer/Texture.h"
#include "../../engine/src/renderer/VisualObject.h"
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
        
        this->cube = this->entityManager.createEntity("Cube");
        
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
        
        VisualObject bomb = {"resources/models/bomb.obj"};
        bomb.loadObject();
        this->cube.addComponent<VisualComponent>(bomb).addComponent<PlacementComponent>(vec3{0.f, 0.f, 0.f});
        
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{3.f, 0.f, 3.f});
        
        CameraComponent cc(vec3{-1.f, 0.f, -1.f}, vec3{0.f, 1.f, 0.f});
        cc.setProjectionMatrix(45, this->window.getAspectRatio(), 0.1f, 100.f);
        cc.setViewMatrix(pcPlayer.getPosition());
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
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
