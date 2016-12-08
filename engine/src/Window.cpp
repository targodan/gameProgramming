#include "Window.h"
#include "WTFException.h"

namespace engine {
    using namespace ogl;
    Window::Window(int width, int height, std::string title) 
        : width(width), height(height), title(title.c_str()) {

        glfwSetErrorCallback(Window::glfwErrorCallback);
        if(!glfwInit()) {
            throw("Couldd not initialize GLFW library.");
        }       

        // TODO: Understand and use window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // Request debug context

        // TODO: Use GLFWmonitor for full screen mode, etc
        this->glfwWindow = glfwCreateWindow(width, height, this->title, NULL, NULL);
        if(!glfwWindow) {
            glfwTerminate();
            throw("Could not create OpenGL window.");
        }
        glfwMakeContextCurrent(this->glfwWindow);
        
        glfwSetErrorCallback(Window::glfwErrorCallback);
        glfwSetWindowSizeCallback(this->glfwWindow, Window::glfwResizeCallback);
        
        int glInit = ogl_LoadFunctions();
        if(glInit != ogl_LOAD_SUCCEEDED) {
            glfwTerminate();
            throw("Could not initialize OpenGL.");
        }
        glClearColor(0.0, 0.0, 0.0, 1.0);

        glfwSwapInterval(1);
        glGetError();
    }
    
    Window::~Window() {
        glfwTerminate();
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
    
    void Window::glfwErrorCallback(int error, const char* description) {
        std::string msg = "Caught GLFW error: " + std::string(description) + std::to_string(error);
        throw WTFException(msg.c_str());
    }
    
    void Window::glfwResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
        glViewport(0, 0, newWidth, newHeight);
    }
}
