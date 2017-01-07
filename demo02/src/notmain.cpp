#include <iostream>
#include <cmath>

#include "glm/gtx/string_cast.hpp"
// GLFW
#include "Game.h"
#include "../../engine/src/renderer/ShaderProgram.h"
#include "../../engine/src/renderer/Camera.h"
#include "../../engine/src/ECS/Entity.h"
#include <GLFW/glfw3.h>
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
#include "../../engine/src/renderer/Camera.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine/src/Window.h"
#include <iostream>

using namespace engine::renderer::gl;
using namespace engine::ECSCommon;
using engine::renderer::Vertex;
using engine::renderer::ShaderProgram;
using glm::vec3;
using engine::util::vector;
using engine::Game;

int main()
{
    engine::Window window = {1024, 768, "test"};
    engine::renderer::Camera cam = {glm::vec3{10.f, 0.f, 10.f}};
    cam.setModelViewMatrix(glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f});
    cam.setProjectionMatrix(45, window.getAspectRatio(),0.1f, 100.f);
    
    auto shaderPtr = std::make_shared<ShaderProgram>("/home/tim/Documents/Code/GameProgramming/demo02/src/triangle_sh.vsh", 
                                                     "/home/tim/Documents/Code/GameProgramming/demo02/src/triangle_sh.fsh");

    auto shaderPtr2 = shaderPtr;
    Material matT = {shaderPtr};
    
    
    glm::vec3 vertexPos = {0.0f, 0.0f, 0.0f};
    vector<Vertex> vertices = {Vertex(vertexPos+glm::vec3{0.f, -1.f, -1.f}), 
                                Vertex(vertexPos+glm::vec3{0.f, -1.f, 1.f}), 
                                Vertex(vertexPos+glm::vec3{0.f, 1.f, 0.f})};
    vector<GLuint> indices = {0, 1, 2};
    Mesh meshH = {vertices, indices};
    meshH.loadMesh();
    meshH.setMaterial(std::make_shared<Material>(matT));
    
    Mesh mesh = meshH;
    Material mat = matT;
    
    // Game loop
    while (window.isOpened())
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate shader
        shaderPtr2->useProgram();
        shaderPtr2->setUniform("projectionMatrix", cam.getProjectionMatrix());
        shaderPtr2->setUniform("modelViewMatrix", cam.getModelViewMatrix());

        mesh.render();

        glfwSwapBuffers(window.getWindow());
    }
    mat.releaseMaterial();
    mesh.releaseMesh();
    glfwTerminate();
    return 0;
}

