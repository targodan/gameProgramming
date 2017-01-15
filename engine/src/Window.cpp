#include "Window.h"
#include "WTFException.h"
#include "GLException.h"

namespace engine {
    using namespace renderer::gl;
    Window::Window(int width, int height, std::string title) 
        : width(width), height(height), title(title.c_str()) {

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
        
        int glInit = ogl_LoadFunctions();
        if(glInit != ogl_LOAD_SUCCEEDED) {
            throw GLException("Failed to initialize OpenGL");
        }
        
        this->setClearColor(0.f, 0.f, 0.f);
        glViewport(0,0, this->width, this->height);
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
    }
}
