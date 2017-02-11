#include "demo03.h"
#include "../../engine/src/physics/constants.h"
#include "../../engine/src/physics/ParticleSystem.h"
#include "ParticleForce.h"
#include <eigen3/Eigen/Eigen>
#include "../../engine/src/ECSCommon.h"
#include "ParticleForce.h"
#include "Actions.h"
#include "../../engine/src/renderer/Mesh.h"
#include "../../engine/src/renderer/Material.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/renderer/gl/gl_core_3_3.h"
#include "../../engine/src/util/vec3.h"
#include "../../engine/src/util/vector.h"
#include <memory>
#include "../../engine/src/renderer/Vertex.h"
#include "../../engine/src/renderer/ElementBuffer.h"
#include "../../engine/src/renderer/Texture.h"
#include "../../engine/src/renderer/TextureParameter.h"
#include "../../engine/src/renderer/DefaultShader.h"
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
    using engine::physics::ParticleSystem;
    using engine::util::vector;
    using glm::vec3;
    using engine::Game;
    using namespace Eigen;
    
    Demo03::Demo03(int argc, char** argv, double ups) 
        : Game(argc, argv, ups), firstMouseMovement(true) {
        this->window.setClearColor(0.f, 0.2f, 0.2f);
              
        this->floor = this->entityManager.createEntity("Floor");
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
        
        this->floor.addComponent<VisualComponent>(std::make_shared<VisualObject>(obj)).addComponent<PlacementComponent>(vec3{0.f, -1.f, 0.f});
        
        
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{-10.f, 0.f, -10.f});
        pcPlayer.setVelocity(vec3{0.f, 0.f, 0.f});
        
        CameraComponent cc(vec3{1.f, 0.f, 1.f}, vec3(0.f, 1.f, 0.f), 90.f, this->window.getAspectRatio(),0.1f, 100.f);
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
        
        this->PatSys = this->entityManager.createEntity("Triangles");
        vector<Vertex> instanceVertices = { Vertex{vec3{-0.2f, -0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{0, 0}}, 
                                            Vertex{vec3{0.2f, -0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{1, 0}},
                                            Vertex{vec3{-0.2f, 0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{0, 1}},
                                            Vertex{vec3{0.2f, 0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{1, 0}},
                                            Vertex{vec3{-0.2f, 0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{1, 1}},
                                            Vertex{vec3{0.2f, -0.2f, 0.f}, vec3{0.f, 0.f, 0.f}, vec2{0, 1}}};
        vector<float> positions(1200, 0.0f);
        auto iMesh = std::make_shared<InstanceMesh>(instanceVertices, positions);
        auto iMat = std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createTextureInstancingVertexShader(), DefaultShader::createTextureInstancingFragmentShader())));
        Texture iTex = {"resources/textures/NeGeo.png"};
        iMat->attachTexture(iTex);
        VisualObject vo = {iMesh, iMat};
        vo.loadObject();
        
        ParticleSystem pats(3, 0.2, iMesh ,ParticleForce::getForceOnVertices(1200));
        std::shared_ptr<ParticleSystem> patsptr = std::make_shared<ParticleSystem>(pats);
        
        this->PatSys.addComponent<VisualComponent>(std::make_shared<VisualObject>(vo)).addComponent<PlacementComponent>(vec3{0.f, -0.5f, 0.f}).addComponent<ParticleSystemComponent>(patsptr);
        
        
    }

    Demo03::~Demo03() {
        
    }
    
    void Demo03::initialize() {
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
        
                    
        auto bombMesh = std::make_shared<VisualObject>("resources/models/bomb.obj");
        bombMesh->loadObject();
        
        auto action6 = std::make_shared<PlaceBombAction>(PlaceBombAction(-2, GLFW_MOUSE_BUTTON_RIGHT, std::make_shared<Entity>(this->player), bombMesh));
        bm.insertMapping(-2, GLFW_MOUSE_BUTTON_RIGHT, action6);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>(this->messageHandler);
        //this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<InputSystem>(bm);
        this->systemManager.enableSystem<ParticleSystemSystem>();
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void Demo03::shutdown() {
        Game::shutdown();
    }
}
