# the compiler: gcc for C program, define as g++ for C++
G++VERSION = $(shell g++ -dumpversion | cut -f1 -d.)
G++_GT_6 = $(shell echo $(G++VERSION)\>=6 | bc)

ifeq ($(G++_GT_6),1) 
	CC := g++
else
	CC := g++-6
endif

ECHO = echo
FIND = find
LN = ln

TAB = $(shell echo -d1 "\t")

BUILDDIR = build
SRCDIR = src
ENGINEDIR = ../engine

# Set to true for debug info.
# Can also be manipulated from terminal via `make DEBUG=true`
DEBUG = false

# compiler flags:
# -Wall turns on most, but not all, compiler warnings
# -fPIC neccessary for shared library (Position Independent Code)
C_FLAGS_COMMON = -std=c++14 -Wall -DELPP_THREAD_SAFE -fPIC -I$(ENGINEDIR)/src -I$(SRCDIR)
LD_FLAGS_COMMON = -L. -lengine -lSOIL -lglfw -lXrandr -lGL -lXi -lX11 -lXxf86vm  -pthread -Wl,-rpath=.,--as-needed 

ifeq ($(CC),g++)
	C_FLAGS_DEBUG+=-fvar-tracking-assignments
else ifeq ($(CC),clang)
	C_FLAGS_COMMON+=-stdlib=libc++14 -Wno-c++11-extensions-warning
	LD_FLAGS_COMMON+=-nodefaultlibs -lstdc++ -lc++ -lc++abi -lgcc_s -lgcc -lc
endif


# -DNDEBUG -DBOOST_UBLAS_NDEBUG turns off boost debug features and improves performance
# -O3 Optimize as much as possible
# -march=native Optimize for the current cpu => program may not run on other cpus without recompilation
C_FLAGS_RELEASE = -DNDEBUG -DBOOST_UBLAS_NDEBUG -O3 -march=native
#  -g    adds debugging information to the executable file
# -Og	optimize a bit, but keep debugging available
C_FLAGS_DEBUG  = -O0 -g -DDEBUG -gdwarf-4
ifeq ($(DEBUG),true)
	C_FLAGS   = $(C_FLAGS_COMMON) $(C_FLAGS_DEBUG)
	LD_FLAGS  = $(LD_FLAGS_COMMON) $(LD_FLAGS_DEBUG)
else
	C_FLAGS   = $(C_FLAGS_COMMON) $(C_FLAGS_RELEASE)
	LD_FLAGS  = $(LD_FLAGS_COMMON) $(LD_FLAGS_RELEASE)
endif

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

# the build target executable:
TARGET = demo02

# All is same as build test
all: lnengine test

build: mkdir $(SOURCES) $(TARGET)

mkdir:
	@$(FIND) . -type d -not -path "*$(BUILDDIR)*" -exec mkdir -p $(BUILDDIR)/{} \;
	
lnengine:
	@$(ECHO) "LN$(TAB)$@"
	@$(LN) -sf $(ENGINEDIR)/libengine.so libengine.so

$(TARGET): $(OBJECTS)
	@$(ECHO) "LD$(TAB)$@"
	@$(CC) -o $@ $^ $(LD_FLAGS)
	
$(BUILDDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@$(ECHO) "CC$(TAB)$@"
	@$(CC) $(C_FLAGS) -c -o $@ $<
	
test: build

clean:
	@$(ECHO) "CLEAN"
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)
	$(RM) libengine.so
