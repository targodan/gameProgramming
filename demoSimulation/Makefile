# the compiler: gcc for C program, define as g++ for C++
CC = g++
PROTOC = protoc
ECHO = echo
MV = mv
FIND = find
LN = ln

TAB = $(shell echo -e "\t")

BUILDDIR = build
SRCDIR = src
TESTDIR = test
ENGINEDIR = ../engine

# Set to true for debug info.
# Can also be manipulated from terminal via `make DEBUG=true`
DEBUG = false

# -Wall turns on most, but not all, compiler warnings
# -fPIC neccessary for shared library (Position Independent Code)
#C_FLAGS_COMMON = -std=c++14 -Wall -pedantic -fPIC -DELPP_THREAD_SAFE
C_FLAGS_COMMON = -std=c++14 -Wall -pedantic -fopenmp `freetype-config --cflags` -DELPP_FEATURE_PERFORMANCE_TRACKING -DELPP_THREAD_SAFE -I$(SRCDIR) -I$(SRCDIR)/engine
# -shared: build a shared library
# -lm -lpthread: Link against math and pthread
# -Wl,--as-needed: feel free to not link if no symbols of a library are needed
LD_FLAGS_COMMON = -L. -lm -lpthread -lprotobuf -lengine `freetype-config --libs` -Wl,-rpath=.,--as-needed

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

C_FLAGS_TEST = $(C_FLAGS_COMMON) $(C_FLAGS_DEBUG) -I$(SRCDIR) -I$(TESTDIR)
LD_FLAGS_TEST = -L. -lm -lcppunit -lengine -lpthread -lprotobuf -lboost_system -lboost_filesystem -Wl,-E,-rpath=.,--as-needed

PROTO_FILES = $(shell find $(SRCDIR) -type f -name '*.proto')
PROTO_GENERATED_H = $(PROTO_FILES:.proto=.pb.h)
PROTO_GENERATED_CC = $(PROTO_FILES:.proto=.pb.cc)

SOURCES_CPP = $(shell find $(SRCDIR) -type f -name '*.cpp')
SOURCES_CC = $(PROTO_GENERATED_CC)
SOURCES = $(SOURCES_CPP) $(SOURCES_CC)
OBJECTS = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

PROTO_FILES_TEST = $(shell find $(TESTDIR) -type f -name '*.proto')
PROTO_GENERATED_TEST_H = $(PROTO_FILES_TEST:.proto=.pb.h)
PROTO_GENERATED_TEST_CC = $(PROTO_FILES_TEST:.proto=.pb.cc)

SOURCES_TEST_CPP = $(shell find $(TESTDIR) -type f -name '*.cpp')
SOURCES_TEST_CC = $(PROTO_GENERATED_TEST_CC)
SOURCES_TEST = $(SOURCES_TEST_CPP) $(SOURCES_TEST_CC)
OBJECTS_TEST = $(addprefix $(BUILDDIR)/,$(addsuffix .o,$(basename $(SOURCES_TEST))))

# Resolve dependecies like header files
DEPDIR = .d
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$(SRCDIR)/$*.Td
DEPFLAGS_TEST = -MT $@ -MMD -MP -MF $(DEPDIR)/$(TESTDIR)/$*.Td
POSTCOMPILE = $(MV) -f $(DEPDIR)/$(SRCDIR)/$*.Td $(DEPDIR)/$(SRCDIR)/$*.d
POSTCOMPILE_TEST = $(MV) -f $(DEPDIR)/$(TESTDIR)/$*.Td $(DEPDIR)/$(TESTDIR)/$*.d

# the build target executable:
TARGET = demoSimulation
TARGET_TEST = runTests

# All is same as build test
all: build build_test test

generate: $(PROTO_GENERATED_H) $(PROTO_GENERATED_CC)
generate_test: $(PROTO_GENERATED_TEST_H) $(PROTO_GENERATED_TEST_CC)

$(SRCDIR)/%.pb.h: $(SRCDIR)/%.proto
	@$(ECHO) "GEN$(TAB)$@"
	@$(PROTOC) -I$(abspath $(SRCDIR)) --cpp_out=$(SRCDIR) $(abspath $<)
$(SRCDIR)/%.pb.cc: $(SRCDIR)/%.proto
	@$(ECHO) "GEN$(TAB)$@"
	@$(PROTOC) -I$(abspath $(SRCDIR)) --cpp_out=$(SRCDIR) $(abspath $<)

$(TESTDIR)/%.pb.h: $(TESTDIR)/%.proto
	@$(ECHO) "GEN$(TAB)$@"
	@$(PROTOC) -I$(abspath $(TESTDIR)) --cpp_out=$(TESTDIR) $(abspath $<)
$(TESTDIR)/%.pb.cc: $(TESTDIR)/%.proto
	@$(ECHO) "GEN$(TAB)$@"
	@$(PROTOC) -I$(abspath $(TESTDIR)) --cpp_out=$(TESTDIR) $(abspath $<)

build: flags mkdir generate $(SOURCES) | lnengine  $(TARGET)

build_test: lnengine flags_test build generate_test $(SOURCES_TEST) $(TARGET_TEST)

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
	@$(ECHO) "LD$(TAB)$@"
	@$(CC) -o $@ $^ $(LD_FLAGS)

$(TARGET_TEST): $(OBJECTS_TEST)
	@$(ECHO) "LD$(TAB)$@"
	@$(CC) -o $@ $^ $(LD_FLAGS_TEST)

$(BUILDDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cc $(DEPDIR)/$(SRCDIR)/%.d
	@$(ECHO) "CC$(TAB)$@"
	@$(CC) $(DEPFLAGS) $(C_FLAGS) -I$(SRCDIR) -c -o $@ $<
	@$(POSTCOMPILE)

$(BUILDDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/$(SRCDIR)/%.d
	@$(ECHO) "CC$(TAB)$@"
	@$(CC) $(DEPFLAGS) $(C_FLAGS) -c -o $@ $<
	@$(POSTCOMPILE)

$(BUILDDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cpp $(DEPDIR)/$(TESTDIR)/%.d
	@$(ECHO) "CC$(TAB)$@"
	@$(CC) $(DEPFLAGS_TEST) $(C_FLAGS_TEST) -c -o $@ $<
	@$(POSTCOMPILE_TEST)

$(BUILDDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cc $(DEPDIR)/$(TESTDIR)/%.d
	@$(ECHO) "CC$(TAB)$@"
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
	@$(RM) $(PROTO_GENERATED_H)
	@$(RM) $(PROTO_GENERATED_CPP)
	@$(RM) $(PROTO_GENERATED_TEST_H)
	@$(RM) $(PROTO_GENERATED_TEST_CPP)

libengine.so:
	@$(ECHO) "BUILDING ENGINE"
	@cd ../engine && $(MAKE) build
	@$(ECHO) "ENGINE BUILT"

lnengine: libengine.so
	@$(ECHO) "LN$(TAB)$@"
	@$(LN) -sf $(ENGINEDIR)/libengine.so libengine.so

$(DEPDIR)/$(SRCDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/$(SRCDIR)/%.d

$(DEPDIR)/$(TESTDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/$(TESTDIR)/%.d

-include $(addprefix $(DEPDIR)/,$(SOURCES_CPP:.cpp=.d))
-include $(addprefix $(DEPDIR)/,$(SOURCES_CC:.cc=.d))
-include $(addprefix $(DEPDIR)/,$(SOURCES_TEST:.cpp=.d))
