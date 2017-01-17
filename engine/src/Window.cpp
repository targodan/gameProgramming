#include "Window.h"
#include "WTFException.h"
#include "GLException.h"
#include "WindowResizeMessage.h"

#include <easylogging++.h>

namespace engine {
    using namespace renderer::gl;
    
    Window* Window::instance = nullptr;
    
    bool Window::cursorInAnyWindowArea = false;
    Window::Window(ECS::MessageHandler& messageHandler, int width, int height, const std::string& title) 
        : messageHandler(messageHandler), width(width), height(height), title(title.c_str()) {

        if(!glfwInit()) {
            throw WTFException("Failed to initialize GLFW");
        }       

        // TODO: Understand and use window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // Request debug context

        // TODO: Use GLFWmonitor for full screen mode, etc
        this->glfwWindow = glfwCreateWindow(width, height, this->title, nullptr, nullptr);
        if(!glfwWindow) {
            throw WTFException("Failed to create window");
        }
        glfwMakeContextCurrent(this->glfwWindow);
        
        glfwSetErrorCallback(Window::glfwErrorCallback);
        glfwSetWindowSizeCallback(this->glfwWindow, Window::glfwResizeCallback);
        glfwSetCursorEnterCallback(this->glfwWindow, Window::glfwCursorEnterCallback);
        
        int glInit = ogl_LoadFunctions();
        if(glInit != ogl_LOAD_SUCCEEDED) {
            throw GLException("Failed to initialize OpenGL");
        }
        
        this->setClearColor(0.f, 0.f, 0.f); // Standard color a window is cleared with on Window::clear()
        glViewport(0,0, this->width, this->height); // Set viewport
        glEnable(GL_DEPTH_TEST); // Enable z-buffer
        
        Window::instance = this;
    }
    
    Window::~Window() {
        glfwTerminate();
    }
    
    float Window::getAspectRatio() const {
        return ((float) this->width) / ((float) this->height);
    }
    
    int Window::getWidth() const {
        return this->width;
    }

    int Window::getHeight() const {
        return this->height;
    }

    std::string Window::getTitle() const {
        return std::string(this->title);
    }
    
    bool Window::isOpened() const {
        return !glfwWindowShouldClose(this->glfwWindow);
    }
    
    GLFWwindow* Window::getWindow() const {
        return this->glfwWindow;
    }
    
    bool Window::isCursorInWindowArea() const {
        return Window::cursorInAnyWindowArea;
    }
    
    void Window::setClearColor(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }
    
    void Window::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void Window::swapBuffers() {
        glfwSwapBuffers(this->glfwWindow);
    }
    
    void Window::glfwErrorCallback(int error, const char* description) {
        std::string msg = "Caught GLFW error: " + std::string(description) + std::to_string(error);
        throw WTFException(msg.c_str());
    }
    
    void Window::glfwResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
        glViewport(0, 0, newWidth, newHeight);
        LOG(INFO) << "resize";
        if(Window::instance != nullptr) {
            Window::instance->messageHandler.queueMessage(std::shared_ptr<ECS::Message>(new WindowResizeMessage(newWidth, newHeight)));
        }
    }
    
    void Window::glfwCursorEnterCallback(GLFWwindow* window, int entered) {
        // TODO: Substitute by sending messages
        
        if(entered) { // Cursor entered window
            Window::cursorInAnyWindowArea = true;
        } else { // Cursor left window
            Window::cursorInAnyWindowArea = false;
        }
    }
}
