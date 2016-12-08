#include "demo02.h"

using namespace demo;
int main(int argc, char** argv) {
    Demo02* demo = new Demo02{argc, argv};
    demo->run();
}
