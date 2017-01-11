#include "demo02.h"
#include "../../engine/src/ECSCommon/PlacementComponent.h"
#include "../../engine/src/ECSCommon/VisualComponent.h"
#include "../../engine/src/ECSCommon/PlacementSystem.h"
#include "../../engine/src/ECSCommon/RenderSystem.h"
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

namespace demo {
    using namespace engine::renderer::gl;
    using namespace engine::ECSCommon;
    using glm::vec3;
    using engine::util::vector;
    using engine::Game;
    
    Demo02::Demo02(int argc, char** argv, double ups) 
        : Game(argc, argv, ups), camera{vec3{10.f, 0.f, 10.f}} {
        // Set camera
        this->camera.setModelViewMatrix(vec3{0.f, 0.f, 0.f}, vec3{0.f, 1.f, 0.f});
        this->camera.setProjectionMatrix(45, this->window.getAspectRatio(),0.1f, 100.f);
        
        // Create triangle entity
        this->triangle = this->entityManager.createEntity("Triangle");
        
        vec3 triangleOrigin = {0.f, 0.f, 0.f};
        PlacementComponent pc;
        pc.setPosition(triangleOrigin);
        pc.setDirection(-(pc.getPosition()));
              
        Material material = {std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh")};
        
        vector<Vertex> vertices = {Vertex(pc.getPosition()+vec3{0.f, -1.f, -1.f}), 
                                   Vertex(pc.getPosition()+vec3{0.f, -1.f, 1.f}), 
                                   Vertex(pc.getPosition()+vec3{0.f, 1.f, 0.f})};
        vector<GLuint> indices = {0, 1, 2};  
        Mesh mesh = {vertices, indices};
        mesh.loadMesh();
        
        this->triangle.addComponent<VisualComponent>(mesh, material);
        this->triangle.addComponent<PlacementComponent>(pc);
    }

    Demo02::~Demo02() {
        
    }
    
    void Demo02::initialize() {
        this->systemManager.enableSystem<engine::ECSCommon::PlacementSystem>();
        this->systemManager.enableSystem<engine::ECSCommon::RenderSystem>();
        Game::initialize();
    }
    
    void Demo02::shutdown() {
        Game::shutdown();
    }
    
    void Demo02::processEvents() {
        glfwPollEvents();
    }

    void Demo02::render(double deltaTimeSeconds) {
        glClearColor(0, 0.2, 0.2, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        auto shaderPtr = dynamic_cast<VisualComponent&>(this->triangle.getComponent(VisualComponent::getComponentTypeId())).getMaterial().getShader();
        shaderPtr->useProgram();
        shaderPtr->setUniform("projectionMatrix", this->camera.getProjectionMatrix());
        shaderPtr->setUniform("modelViewMatrix", this->camera.getModelViewMatrix());
        
        
        Game::render(deltaTimeSeconds);
        glfwSwapBuffers(this->window.getWindow());
    }

    void Demo02::update(double deltaTimeSeconds) {
    }
}
