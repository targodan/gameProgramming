set breakpoint pending on
b SystemManager.cpp:42
commands
    print i
    print j
    call this->dbg_printSystems()
end
b SystemManagerTest.h:206
