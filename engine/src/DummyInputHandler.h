#ifndef DUMMYINPUTHANDLER_H
#define DUMMYINPUTHANDLER_H

#include "../../lib/glfw/include/GLFW/glfw3.h"

// Note: Absolutely a dummy class, used for mere test purposes
class DummyInputHandler {
private:
    GLFWwindow* window;
public:
    DummyInputHandler(GLFWwindow* window);
    DummyInputHandler(const DummyInputHandler& orig) = delete;
    virtual ~DummyInputHandler();
    
    void handle();
    bool isKeyPressed(int key) const;
};

#endif /* DUMMYINPUTHANDLER_H */

