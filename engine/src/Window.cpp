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
        this->glfwWindow = glfwCreateWindow(width, this->height, this->title, nullptr, nullptr);
        if(!this->glfwWindow) {
            throw WTFException("Failed to create window. Errorcode: 0x%08zX", reinterpret_cast<size_t>(this->glfwWindow));
        }
        glfwMakeContextCurrent(this->glfwWindow);
        
        glfwSetErrorCallback(Window::glfwErrorCallback);
        glfwSetFramebufferSizeCallback(this->glfwWindow, Window::glfwFramebufferResizeCallback);
        glfwSetCursorEnterCallback(this->glfwWindow, Window::glfwCursorEnterCallback);
        
        int glInit = ogl_LoadFunctions();
        if(glInit != ogl_LOAD_SUCCEEDED) {
            throw GLException("Failed to initialize OpenGL");
        }
        
        this->setClearColor(0.f, 0.f, 0.f); // Standard color a window is cleared with on Window::clear()
        
        /*
         * Reason for changes:
         * > While the size of a window is measured in screen coordinates,
         * > OpenGL works with pixels. The size you pass into glViewport,
         * > for example, should be in pixels. On some machines screen
         * > coordinates and pixels are the same, but on others they will not be.
         * > There is a second set of functions to retrieve the size, in pixels,
         * > of the framebuffer of a window.
         * 
         * Source: http://www.glfw.org/docs/latest/window_guide.html
         */
        glfwGetFramebufferSize(this->glfwWindow, &this->width, &this->height);
        glViewport(0, 0, this->width, this->height);
        
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
    
    void Window::glfwFramebufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
        glViewport(0, 0, newWidth, newHeight);
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
