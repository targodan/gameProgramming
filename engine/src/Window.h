#ifndef WINDOW_H
#define WINDOW_H

#include "renderer/gl/gl_core_3_3.h"
#include "ECS/MessageHandler.h"
#include <GLFW/glfw3.h>

#include <string>

namespace engine {
    using namespace renderer::gl;	

    class Window {
    private:
        GLFWwindow* glfwWindow;
        
        ECS::MessageHandler& messageHandler;
        
        int width;
        int height;

        const char* title;
        
        // only for the resize callback
        // TODO: make pretty
        static Window* instance;

        static void glfwErrorCallback(int error, const char* description);
        static void glfwFramebufferResizeCallback(GLFWwindow* window, int newWidth, int newHeight);
        static void glfwCursorEnterCallback(GLFWwindow* window, int entered);
        static bool cursorInAnyWindowArea; // Temporary cheat
    public:
        Window(ECS::MessageHandler& messageHandler, int width, int height, const std::string& title = {""});
        Window(const Window& orig) = delete;
        ~Window();
        
        float getAspectRatio() const;
        int getWidth() const;
        int getHeight() const;
        std::string getTitle() const;
        GLFWwindow* getWindow() const;
        
        bool isCursorInWindowArea() const;
        
        void setClearColor(float red, float green, float blue, float alpha=1.f);
        
        // This has to be done before rendering
        void clear();
        
        // This has to be done after rendering
        void swapBuffers();
        void setVSync(bool on);
        
        bool isOpened() const;
    };
}

#endif /* WINDOW_H */

