# the compiler: gcc for C program, define as g++ for C++
CC = g++
ECHO = echo
MV = mv
FIND = find

# Set to true for debug info.
# Can also be manipulated from terminal via `make DEBUG=true`
DEBUG = false

# -Wall turns on most, but not all, compiler warnings
# -fPIC neccessary for shared library (Position Independent Code)
#C_FLAGS_COMMON = -std=c++14 -Wall -pedantic -fPIC -DELPP_THREAD_SAFE
C_FLAGS_COMMON = -std=c++14 -Wall -pedantic -fPIC
# -shared: build a shared library
# -lm -lpthread: Link against math and pthread
# -Wl,--as-needed: feel free to not link if no symbols of a library are needed
LD_FLAGS_COMMON = -shared -lm -lpthread -Wl,--as-needed

ifeq ($(CC),g++)
	C_FLAGS_DEBUG+=-fvar-tracking-assignments
else ifeq ($(CC),clang)
	C_FLAGS_COMMON+=-stdlib=libc++14 -Wno-c++11-extensions-warning
	LD_FLAGS_COMMON+=-nodefaultlibs -lstdc++ -lc++ -lc++abi -lgcc_s -lgcc -lc
endif

# -DNDEBUG -DBOOST_UBLAS_NDEBUG turns off boost debug features and improves performance
# -O3 Optimize as much as possible
# -march=native Optimize for the current cpu => program may not run on other cpus without recompilation
C_FLAGS_RELEASE = -DNDEBUG -DBOOST_UBLAS_NDEBUG -O3 -march=native -msse2
# -g    adds debugging information to the executable file
# -gdwarf4: use the newer v4 format for debugging information
C_FLAGS_DEBUG = -O0 -g -DDEBUG -gdwarf-4
# -Wl,-O3: optimize as hard as you can
LD_FLAGS_RELEASE = -Wl,-O3
# -rdynamic: dunno, people say it makes debugging better
LD_FLAGS_DEBUG = -rdynamic

ifeq ($(DEBUG),true)
	C_FLAGS   = $(C_FLAGS_COMMON) $(C_FLAGS_DEBUG)
	LD_FLAGS  = $(LD_FLAGS_COMMON) $(LD_FLAGS_DEBUG)
else
	C_FLAGS   = $(C_FLAGS_COMMON) $(C_FLAGS_RELEASE)
	LD_FLAGS  = $(LD_FLAGS_COMMON) $(LD_FLAGS_RELEASE)
endif

C_FLAGS_TEST = $(C_FLAGS_DEBUG) -I./src
LD_FLAGS_TEST = -L. -lm -lcppunit -lengine -lpthread -lboost_system -lboost_filesystem -Wl,-E,-rpath=.,--as-needed

BUILDDIR = build

SOURCES = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJECTS = $(addprefix $(BUILDDIR)/,$(SOURCES:.cpp=.o))

SOURCES_TEST = $(wildcard test/*.cpp) $(wildcard test/**/*.cpp)
OBJECTS_TEST = $(addprefix $(BUILDDIR)/,$(SOURCES_TEST:.cpp=.o))

# Resolve dependecies like header files
DEPDIR = .d
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/src/$*.Td
DEPFLAGS_TEST = -MT $@ -MMD -MP -MF $(DEPDIR)/test/$*.Td
POSTCOMPILE = $(MV) -f $(DEPDIR)/src/$*.Td $(DEPDIR)/src/$*.d
POSTCOMPILE_TEST = $(MV) -f $(DEPDIR)/test/$*.Td $(DEPDIR)/test/$*.d

# the build target executable:
TARGET = libengine.so
TARGET_TEST = runTests

# All is same as build test
all: test

build: flags mkdir $(SOURCES) $(TARGET)

build_test: flags_test build $(SOURCES_TEST) $(TARGET_TEST)

mkdir:
	@$(FIND) . -type d -not -path "*$(BUILDDIR)*" -exec mkdir -p $(BUILDDIR)/{} \;
	@$(FIND) . -type d -not -path "*$(DEPDIR)*" -exec mkdir -p $(DEPDIR)/{} \;

flags:
	@$(ECHO) "C_FLAGS = $(C_FLAGS)"
	@$(ECHO) "LD_FLAGS = $(LD_FLAGS)"

flags_test:
	@$(ECHO) "C_FLAGS_TEST = $(C_FLAGS_TEST)"
	@$(ECHO) "LD_FLAGS_TEST = $(LD_FLAGS_TEST)"

$(TARGET): $(OBJECTS)
	@$(ECHO) "LD $@"
	@$(CC) -o $@ $^ $(LD_FLAGS) 

$(TARGET_TEST): $(TARGET) $(OBJECTS_TEST)
	@$(ECHO) "LD $@"
	@$(CC) -o $@ $^ $(LD_FLAGS_TEST) 

$(BUILDDIR)/src/%.o: src/%.cpp $(DEPDIR)/src/%.d
	@$(ECHO) "CC $@"
	@$(CC) $(DEPFLAGS) $(C_FLAGS) -c -o $@ $<
	@$(POSTCOMPILE)

$(BUILDDIR)/test/%.o: test/%.cpp $(DEPDIR)/test/%.d
	@$(ECHO) "CC $@"
	@$(CC) $(DEPFLAGS_TEST) $(C_FLAGS_TEST) -c -o $@ $<
	@$(POSTCOMPILE_TEST)

test: build_test $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	@$(ECHO) "CLEAN"
	@$(RM) $(OBJECTS)
	@$(RM) $(TARGET)
	@$(RM) $(OBJECTS_TEST)
	@$(RM) $(TARGET_TEST)


$(DEPDIR)/src/%.d: ;
.PRECIOUS: $(DEPDIR)/src/%.d

$(DEPDIR)/test/%.d: ;
.PRECIOUS: $(DEPDIR)/src/%.d

-include $(addprefix $(DEPDIR)/,$(SOURCES:.cpp=.d))
-include $(addprefix $(DEPDIR)/,$(SOURCES_TEST:.cpp=.d))
