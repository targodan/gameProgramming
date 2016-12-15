GCC = g++
MKDIR = mkdir

# CPP_OPENGL_PKG = `pkg-config --cflags glfw3`
# LD_OPENGL_PKG = `pkg-config --libs glfw3`

CPP_FLAGS = -std=c++14 -Wall -O2
LD_FLAGS = -lglfw -lXrandr -lGL -lXi -pthread -lm -lX11 -lXxf86vm

CPP_GLLOADGEN = -Wno-write-strings

BUILDDIR = build
TARGET = demoGL

SOURCES = $(wildcard *.cpp) 
OBJECTS = $(addprefix $(BUILDDIR)/, $(SOURCES:.cpp=.o)) 


all: mkdir $(SOURCES) $(TARGET)

mkdir:
	find . -type d -not -path "*$(BUILDDIR)*" -exec $(MKDIR) -p $(BUILDDIR)/{} \;

$(TARGET): $(OBJECTS)
	$(GCC) -o $@ $^ $(LD_FLAGS)
	
$(BUILDDIR)/%.o: %.cpp 
	$(GCC) $(CPP_FLAGS) -c -o $@ $< 

clean:
	rm -rf $(BUILDDIR)/*
	rm $(TARGET)
