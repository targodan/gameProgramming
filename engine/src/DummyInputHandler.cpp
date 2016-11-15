#include "DummyInputHandler.h"

DummyInputHandler::DummyInputHandler(GLFWwindow* window) : window(window) {
    
}

void DummyInputHandler::handle() {
    glfwPollEvents();
}

bool DummyInputHandler::isKeyPressed(int key) const {
    return glfwGetKey(this->window, key) == GLFW_PRESS;
}