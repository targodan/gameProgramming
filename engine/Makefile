# the compiler: gcc for C program, define as g++ for C++
CC = g++

# Set to true for debug info.
# Can also be manipulated from terminal via `make DEBUG=true`
DEBUG = false

# compiler flags:
# -Wall turns on most, but not all, compiler warnings
# -fPIC neccessary for shared library (Position Independent Code)
C_FLAGS_COMMON = -std=c++14 -Wall -fPIC
# -DNDEBUG -DBOOST_UBLAS_NDEBUG turns off boost debug features and improves performance
# -O3 Optimize as much as possible
# -march=native Optimize for the current cpu => program may not run on other cpus without recompilation
C_FLAGS_RELEASE = $(C_FLAGS_COMMON) -DNDEBUG -DBOOST_UBLAS_NDEBUG -O3 -march=native
# -g    adds debugging information to the executable file
# -Og	optimize a bit, but keep debugging available
C_FLAGS_DEBUG = $(C_FLAGS_COMMON) -g -Og
ifeq ($(DEBUG),true)
	C_FLAGS  = $(C_FLAGS_DEBUG)
else
	C_FLAGS  = $(C_FLAGS_RELEASE)
endif
LD_FLAGS = -shared
BUILDDIR = build

SOURCES = $(wildcard *.cpp) $(wildcard **/*.cpp)
OBJECTS = $(addprefix $(BUILDDIR)/,$(SOURCES:.cpp=.o))

# the build target executable:
TARGET = libengine.so

# All is same as build test
all: test

build: mkdir $(SOURCES) $(TARGET)

mkdir:
	@find . -type d -not -name $(BUILDDIR) -exec mkdir -p $(BUILDDIR)/{} \;

$(TARGET): $(OBJECTS)
	$(CC) $(LD_FLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(C_FLAGS) -c -o $@ $<

test: build
	@echo Tests are not implemented yet.

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)
