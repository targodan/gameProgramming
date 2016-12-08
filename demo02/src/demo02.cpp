#include "demo02.h"
#include "renderer/glloadgen/gl_core_3_3.h"

namespace demo {
    using namespace ogl;
    
    Demo02::Demo02(int argc, char** argv, double ups) 
        : Game(argc, argv, ups) {
        
    }

    Demo02::~Demo02() {

    }


    void Demo02::processEvents() {
        glfwPollEvents();
    }


    void Demo02::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(this->window.getWindow());
    }



    void Demo02::update() {
    }
}
