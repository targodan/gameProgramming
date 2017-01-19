#include "demo02.h"
#include "../../engine/src/ECSCommon.h"
#include "../../engine/src/renderer/Mesh.h"
#include "../../engine/src/renderer/Material.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/renderer/gl/gl_core_3_3.h"
#include "../../engine/src/util/vec3.h"
#include "../../engine/src/util/vector.h"
#include "../../engine/src/renderer/Vertex.h"
#include "../../engine/src/renderer/ElementBuffer.h"
#include "../../engine/src/renderer/Texture.h"
#include "../../engine/src/renderer/TextureParameter.h"
#include <iostream>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

namespace demo {
    using namespace engine::renderer::gl;
    using namespace engine::ECSCommon;
    using namespace engine::renderer;
    using glm::vec2;
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
        pc.setDirection(vec3{0.f, 0.f, 0.f});
        
        Texture texture = {"src/media/container.jpg"};
        vector<Texture> textures = {texture};
        Material material = {std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"), textures};
        material.loadTextures();
        
        // TODO: Camera wrong way around?!
        vector<Vertex> vertices = {Vertex(pc.getPosition()+vec3{0.f, -1.f, -1.f}, vec3{1.f, 0.f, 0.f}, vec2{1.f, 1.f}), // bottom right ?!
                                   Vertex(pc.getPosition()+vec3{0.f, -1.f, 1.f}, vec3{0.f, 1.f, 0.f}, vec2{0.f, 1.f}),  // bottom left ?!
                                   Vertex(pc.getPosition()+vec3{0.f, 1.f, -1.f}, vec3{0.f, 0.f, 1.f}, vec2{1.f, 0.f}),  // top right ?!
                                   Vertex(pc.getPosition()+vec3{0.f, 1.f, 1.f}, vec3{1.f, 0.f, 0.f}, vec2{0.f, 0.f})};  // top left ?!
        vector<GLuint> indices = {2, 0, 3,
                                  0, 1, 3};
        Mesh mesh = {vertices, indices};
        mesh.loadMesh();
       
        this->triangle.addComponent<VisualComponent>(mesh, material);
        this->triangle.addComponent<PlacementComponent>(pc);
        
        // Create player entity
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{10.f, 0.f, 0.f});
        pcPlayer.setDirection(vec3{0.f, 0.f, 0.f});
        
        CameraComponent cc(vec3{-10.f, 0.f, 0.f}, vec3{0.f, 1.f, 0.f});
        cc.setProjectionMatrix(45, this->window.getAspectRatio(),0.1f, 100.f);
        cc.setViewMatrix(pcPlayer.getPosition());
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
    }

    Demo02::~Demo02() {
        
    }
    
    void Demo02::initialize() {
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    
    void Demo02::shutdown() {
        Game::shutdown();
    }
    
    void Demo02::processEvents() {
        glfwPollEvents();
        
        double newX, newY;
        glfwGetCursorPos(this->window.getWindow(), &newX, &newY);
        
        if(this->firstMouseMovement) {
            this->lastX = newX;
            this->lastY = newY;
            this->firstMouseMovement = false;
        }
        
        if(this->window.isCursorInWindowArea()) {
            auto& cam = this->player.getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();
            cam.pan(newX-lastX, -(newY-lastY), 0.05f);
        }
        
        this->lastX = newX;
        this->lastY = newY;
    }

    void Demo02::render(double deltaTimeSeconds) {
        Game::render(deltaTimeSeconds);
    }

    void Demo02::update(double deltaTimeSeconds) {
    }
}
