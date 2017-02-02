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
#include <vector>
#include <memory>
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
#include "IO/ButtonMapping.h"

namespace demo {
    using namespace engine::renderer::gl;
    using namespace engine::ECSCommon;
    using namespace IO;
    using engine::physics::ParticleSystem;
    using glm::vec3;
    using engine::Game;
    using namespace Eigen;
    
    Demo03::Demo03(int argc, char** argv, double ups) 
        : Game(argc, argv, ups), firstMouseMovement(true) {
        this->window.setClearColor(0.f, 0.2f, 0.2f);
              
        
        Vertex front( {0.1, 0, 0},        {0, 1, 0});
        Vertex backRight(    {0.1, 0, -0.1},    {0, 0, 1});
        Vertex backLeft(   {-0.1, 0, -0.1},     {1, 0, 0});
        Vertex up(          {0, 0.2, 0},          {1, 1, 0});
        
        std::shared_ptr<Material> material = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", "src/triangle_sh.fsh"), true);
        
        std::vector<Mesh> tetras;
        
        for(int i=0; i<NUM_OF_PARTICLES; i++){
            Mesh tetrahedronMesh({front, backRight, backLeft, up},
                {0, 1, 3, 1, 2, 3, 2, 0, 3, 0, 2, 1},
                DataUsagePattern::DYNAMIC_DRAW);
            auto vc = this->entityManager.createEntity("Tetrahedron" + i)
                .addComponent<VisualComponent>(tetrahedronMesh, *material)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
            vc.getComponent<VisualComponent>().getMesh().loadMesh();
            tetras.push_back(vc.getComponent<VisualComponent>().getMesh());
        }
        
        VectorXf pos = VectorXf::Zero(3*NUM_OF_PARTICLES);
        
        ParticleSystem pats(2, 0.5, tetras, pos, ParticleForce::getForceOnVertices(3*NUM_OF_PARTICLES));
        std::shared_ptr<ParticleSystem> patsptr = std::make_shared<ParticleSystem>(pats);
        
        this->entityManager.createEntity("pew").addComponent<ParticleSystemComponent>(patsptr);
        
        this->player = this->entityManager.createEntity("Player");
        
        PlacementComponent pcPlayer;
        pcPlayer.setPosition(vec3{-10.f, 0.f, -10.f});
        pcPlayer.setVelocity(vec3{0.f, 0.f, 0.f});
        
        CameraComponent cc(vec3{1.f, 0.f, 1.f}, vec3{0.f, 1.f, 0.f});
        cc.setProjectionMatrix(45, this->window.getAspectRatio(),0.1f, 100.f);
        cc.setViewMatrix(pcPlayer.getPosition());
        
        this->player.addComponent<CameraComponent>(cc).addComponent<PlacementComponent>(pcPlayer);
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
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<InputSystem>(bm);
        this->systemManager.enableSystem<ParticleSystemSystem>();
        Game::initialize();
        
        glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    void Demo03::shutdown() {
        Game::shutdown();
    }
}
