# the compiler: gcc for C program, define as g++ for C++
CC = g++

# Set to true for debug info.
# Can also be manipulated from terminal via `make DEBUG=true`
DEBUG = false

# compiler flags:
C_FLAGS_COMMON = -Wall -fPIC
# -DNDEBUG -DBOOST_UBLAS_NDEBUG turns off boost debug features and improves performance
C_FLAGS_RELEASE = $(C_FLAGS_COMMON) -DNDEBUG -DBOOST_UBLAS_NDEBUG
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
C_FLAGS_DEBUG = $(C_FLAGS_COMMON) -g
ifeq ($(DEBUG),true)
	C_FLAGS  = $(C_FLAGS_DEBUG)
else
	C_FLAGS  = $(C_FLAGS_RELEASE)
endif
LD_FLAGS = -shared
BUILDDIR = build

SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(BUILDDIR)/,$(notdir $(SOURCES:.cpp=.o)))

# the build target executable:
TARGET = libengine.so

# All is same as build test
all: test

build: mkdir $(SOURCES) $(TARGET)

mkdir:
	@mkdir $(BUILDDIR) &>/dev/null || true

$(TARGET): $(OBJECTS)
	$(CC) $(LD_FLAGS) -o $@ $<

$(BUILDDIR)/%.o: %.cpp
	$(CC) $(C_FLAGS) -c -o $@ $<

test: build
	@echo Tests are not implemented yet.

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)
