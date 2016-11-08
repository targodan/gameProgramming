set breakpoint pending on
b SystemManager.cpp:178
commands
    print listIndex
    print parentIndex
    call this->dbg_printSystems()
end
disable breakpoints 1
b SystemManager.cpp:26
