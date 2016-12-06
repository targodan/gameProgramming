#ifndef WINDOW_H
#define WINDOW_H

#include "renderer/glloadgen/gl_core_3_3.h"
#include <GLFW/glfw3.h>

#include <string>

namespace engine {
    using namespace ogl;	

    class Window {
    private:
        GLFWwindow* glfwWindow;

        int width;
        int height;

        const char* title;

        static void glfwErrorCallback(int error, const char* description);
    public:
        Window(int width, int height, std::string title = {"ExplosionBoy"});
        Window(const Window& orig) = delete;
        ~Window();

        int getWidth() const;
        int getHeight() const;
        std::string getTitle() const;
        GLFWwindow* getWindow() const;
        
        bool isOpened() const;
    };
}

#endif /* WINDOW_H */

