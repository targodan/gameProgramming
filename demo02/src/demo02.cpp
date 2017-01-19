#include "demo02.h"
#include "../../engine/src/ECSCommon.h"
#include "Actions.h"
#include "../../engine/src/renderer/Mesh.h"
#include "../../engine/src/renderer/Material.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/renderer/gl/gl_core_3_3.h"
#include "../../engine/src/util/vec3.h"
#include "../../engine/src/util/vector.h"
#include "../../engine/src/renderer/Vertex.h"
#include "../../engine/src/renderer/ElementBuffer.h"
#include <iostream>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"
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
        
        // Create triangle entity
        this->triangle = this->entityManager.createEntity("Triangle");
        
        vec3 triangleOrigin = {0.f, 0.f, 0.f};
        PlacementComponent pc;
        pc.setPosition(triangleOrigin);
        pc.setVelocity(vec3{0.f, 0.f, 0.f});
              
        Material material = {std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh")};
        
        vector<Vertex> vertices = {Vertex(pc.getPosition()+vec3{0.f, -1.f, -1.f}), 
                                   Vertex(pc.getPosition()+vec3{0.f, -1.f, 1.f}), 
                                   Vertex(pc.getPosition()+vec3{0.f, 1.f, 0.f}),
                                   Vertex(pc.getPosition()+vec3{-1.f, -1.f, 0.f})};
        vector<GLuint> indices = {0, 1, 2};  
        Mesh mesh = {vertices, indices};
        mesh.loadMesh();
       
        this->triangle.addComponent<VisualComponent>(mesh, material);
        this->triangle.addComponent<PlacementComponent>(pc);
        
        // Create player entity
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{10.f, 0.f, 0.f});
        pcPlayer.setVelocity(vec3{0.f, 0.f, 0.f});
        
        CameraComponent cc(vec3{-10.f, 0.f, 0.f}, vec3{0.f, 1.f, 0.f});
        cc.setProjectionMatrix(45, this->window.getAspectRatio(),0.1f, 100.f);
        cc.setViewMatrix(pcPlayer.getPosition());
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
       
        
        
//        while(this->window.isOpened()) {
//            glfwPollEvents();
//            glClearColor(0, 0.2f, 0.2f, 1.f);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            
//            auto cam = dynamic_cast<CameraComponent&>(this->player.getComponent(CameraComponent::getComponentTypeId()));
//            auto mesh = dynamic_cast<VisualComponent&>(this->triangle.getComponent(VisualComponent::getComponentTypeId())).getMesh();
//            auto shaderPtr = dynamic_cast<VisualComponent&>(this->triangle.getComponent(VisualComponent::getComponentTypeId())).getMaterial().getShader();
//
//            shaderPtr->useProgram();
//            shaderPtr->setUniform("projectionMatrix", cam.getProjectionMatrix());
//            shaderPtr->setUniform("viewMatrix", cam.getViewMatrix());
//            std::cout << "projection: " << glm::to_string(cc.getProjectionMatrix()) << std::endl;
//            std::cout << "view: " << glm::to_string(cc.getViewMatrix()) << std::endl;
//            
//            mesh.render();
//            glfwSwapBuffers(this->window.getWindow());
//        }
    }

    Demo02::~Demo02() {
        
    }
    
    void Demo02::initialize() {
        auto action1 = std::make_shared<PanCameraAction>(PanCameraAction(-2, -1, std::make_shared<Entity>(this->player)));
        ButtonMapping bm(this->window.getWindow());
        bm.insertMapping(-2, -1, action1);
        auto action2 = std::make_shared<MoveFwdBwdAction>(MoveFwdBwdAction(-1, GLFW_KEY_W, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_W, action2);
        bm.insertMapping(-1, GLFW_KEY_S, action2, true);
        auto action3 = std::make_shared<MoveLRAction>(MoveLRAction(-1, GLFW_KEY_A, std::make_shared<Entity>(this->player)));
        bm.insertMapping(-1, GLFW_KEY_A, action3, true);
        bm.insertMapping(-1, GLFW_KEY_D, action3);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<InputSystem>(bm);
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void Demo02::shutdown() {
        Game::shutdown();
    }
    
    void Demo02::processEvents() {
        glfwPollEvents();
       /* 
        double newX, newY;
        glfwGetCursorPos(this->window.getWindow(), &newX, &newY);
        
        if(this->firstMouseMovement) {
            this->lastX = newX;
            this->lastY = newY;
            this->firstMouseMovement = false;
        }
        
        if(this->window.isCursorInWindowArea()) {
            auto& cam = dynamic_cast<CameraComponent&>(this->player.getComponent(CameraComponent::getComponentTypeId()));
            cam.pan(newX-lastX, -(newY-lastY), 0.05f);
        }
        
        this->lastX = newX;
        this->lastY = newY; */
    }

    void Demo02::render(double deltaTimeSeconds) {
        Game::render(deltaTimeSeconds);
    }

    void Demo02::update(double deltaTimeSeconds) {
    }
}
