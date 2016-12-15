#include "gl_core_3_3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;
using namespace gl;

#define DEBUG 0

glm::mat4 createFrustum(float phi, float aspectRatio, float near, float far) { // float whereeeveer you aare
    float top = near*tan(phi/2.f);
    float bottom = -top;
    float left = bottom*aspectRatio;
    float right = top*aspectRatio;
    
    return glm::frustum(left, right, bottom, top, near, far);
}

GLFWwindow* createWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if(DEBUG) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    return window;
}

bool glInit() {
    return ogl_LoadFunctions();
}

void run(GLFWwindow* window) {
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwSwapBuffers(window);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void resizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
}

int main(int argc, char** argv) {
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        exit(-1);
    }
    
    GLFWwindow* window = createWindow();
    if(window == nullptr) {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        exit(-1);
    }
    
    if(glInit() == ogl_LOAD_FAILED) {
        cout << "Failed to initialize OpenGL" << endl;
        glfwTerminate();
        exit(-1);
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
    
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    glClearColor(0.f, 0.2f, 0.2f, 1.f);
    glEnable(GL_DEPTH_TEST);
    
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    resizeCallback(window, width, height);
    
    run(window);
    
    glfwTerminate();
    return 0;
}
