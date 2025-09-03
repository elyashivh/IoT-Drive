# Project Makefile — builds master & three minions (debug + release)
# Directory layout is taken from the user's tree.

# === Compilers & tools ===
CXX := g++
CC  := gcc
AR  := ar

# === Flags ===
CXXWARN := -Wall -Wextra -Wpedantic
CWARN   := -Wall -Wextra -Wpedantic
COMMON_DEFS := -fPIC -std=gnu++17
INCLUDES := -I concrete/include -I framework/include -I buse
LDFLAGS  := -pthread

DBG_FLAGS := -g -O0
REL_FLAGS := -O3 -DNDEBUG

# === Layout ===
BIN_DEBUG   := bin/debug
BIN_RELEASE := bin/release
DBG_OBJ_DIR := $(BIN_DEBUG)/obj
REL_OBJ_DIR := $(BIN_RELEASE)/obj

# Source discovery (only library/implementation code; tests are linked separately)
CONCRETE_CPP_SRCS := $(wildcard concrete/src/*.cpp)
CONCRETE_C_SRCS   := $(wildcard concrete/src/*.c)
FRAMEWORK_CPP_SRCS := $(wildcard framework/src/*.cpp)
FRAMEWORK_C_SRCS   := $(wildcard framework/src/*.c)

COMMON_CPP_SRCS := $(CONCRETE_CPP_SRCS) $(FRAMEWORK_CPP_SRCS)
COMMON_C_SRCS   := $(CONCRETE_C_SRCS)   $(FRAMEWORK_C_SRCS)

# Object files (mirror the folder structure under obj/)
DBG_OBJS_CPP := $(addprefix $(DBG_OBJ_DIR)/,$(COMMON_CPP_SRCS:.cpp=.o))
DBG_OBJS_C   := $(addprefix $(DBG_OBJ_DIR)/,$(COMMON_C_SRCS:.c=.o))
DBG_OBJS     := $(DBG_OBJS_CPP) $(DBG_OBJS_C)

REL_OBJS_CPP := $(addprefix $(REL_OBJ_DIR)/,$(COMMON_CPP_SRCS:.cpp=.o))
REL_OBJS_C   := $(addprefix $(REL_OBJ_DIR)/,$(COMMON_C_SRCS:.c=.o))
REL_OBJS     := $(REL_OBJS_CPP) $(REL_OBJS_C)

# === Final artifacts ===
DBG_MASTER_OUT   := $(BIN_DEBUG)/master.out
DBG_MINION1_OUT  := $(BIN_DEBUG)/minion1.out
DBG_MINION2_OUT  := $(BIN_DEBUG)/minion2.out
DBG_MINION3_OUT  := $(BIN_DEBUG)/minion3.out

REL_MASTER_OUT   := $(BIN_RELEASE)/master.out
REL_MINION1_OUT  := $(BIN_RELEASE)/minion1.out
REL_MINION2_OUT  := $(BIN_RELEASE)/minion2.out
REL_MINION3_OUT  := $(BIN_RELEASE)/minion3.out

ALL_DEBUG   := $(DBG_MASTER_OUT) $(DBG_MINION1_OUT) $(DBG_MINION2_OUT) $(DBG_MINION3_OUT)
ALL_RELEASE := $(REL_MASTER_OUT) $(REL_MINION1_OUT) $(REL_MINION2_OUT) $(REL_MINION3_OUT)

# === Entrypoints (mains) ===
MASTER_MAIN := concrete/test/master.cpp
MINION_MAIN := concrete/test/minion.cpp

.PHONY: all debug release clean tree

all: debug release

debug: $(ALL_DEBUG)
release: $(ALL_RELEASE)

# --- Link rules (DEBUG) ---
$(DBG_MASTER_OUT): $(DBG_OBJS) $(MASTER_MAIN) | $(BIN_DEBUG)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(DBG_FLAGS) $(INCLUDES) $(MASTER_MAIN) $(DBG_OBJS) -o $@ $(LDFLAGS)

$(DBG_MINION1_OUT): $(DBG_OBJS) $(MINION_MAIN) | $(BIN_DEBUG)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(DBG_FLAGS) $(INCLUDES) -DMINION_ID=1 $(MINION_MAIN) $(DBG_OBJS) -o $@ $(LDFLAGS)

$(DBG_MINION2_OUT): $(DBG_OBJS) $(MINION_MAIN) | $(BIN_DEBUG)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(DBG_FLAGS) $(INCLUDES) -DMINION_ID=2 $(MINION_MAIN) $(DBG_OBJS) -o $@ $(LDFLAGS)

$(DBG_MINION3_OUT): $(DBG_OBJS) $(MINION_MAIN) | $(BIN_DEBUG)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(DBG_FLAGS) $(INCLUDES) -DMINION_ID=3 $(MINION_MAIN) $(DBG_OBJS) -o $@ $(LDFLAGS)

# --- Link rules (RELEASE) ---
$(REL_MASTER_OUT): $(REL_OBJS) $(MASTER_MAIN) | $(BIN_RELEASE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(REL_FLAGS) $(INCLUDES) $(MASTER_MAIN) $(REL_OBJS) -o $@ $(LDFLAGS)

$(REL_MINION1_OUT): $(REL_OBJS) $(MINION_MAIN) | $(BIN_RELEASE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(REL_FLAGS) $(INCLUDES) -DMINION_ID=1 $(MINION_MAIN) $(REL_OBJS) -o $@ $(LDFLAGS)

$(REL_MINION2_OUT): $(REL_OBJS) $(MINION_MAIN) | $(BIN_RELEASE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(REL_FLAGS) $(INCLUDES) -DMINION_ID=2 $(MINION_MAIN) $(REL_OBJS) -o $@ $(LDFLAGS)

$(REL_MINION3_OUT): $(REL_OBJS) $(MINION_MAIN) | $(BIN_RELEASE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(REL_FLAGS) $(INCLUDES) -DMINION_ID=3 $(MINION_MAIN) $(REL_OBJS) -o $@ $(LDFLAGS)

# --- Compilation (DEBUG) ---
$(DBG_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(DBG_FLAGS) $(INCLUDES) -c $< -o $@

$(DBG_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC)  $(CWARN)   $(DBG_FLAGS) $(INCLUDES) -c $< -o $@

# --- Compilation (RELEASE) ---
$(REL_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXWARN) $(COMMON_DEFS) $(REL_FLAGS) $(INCLUDES) -c $< -o $@

$(REL_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC)  $(CWARN)   $(REL_FLAGS) $(INCLUDES) -c $< -o $@

# --- Convenience ---
$(BIN_DEBUG):
	@mkdir -p $(BIN_DEBUG) $(DBG_OBJ_DIR)

$(BIN_RELEASE):
	@mkdir -p $(BIN_RELEASE) $(REL_OBJ_DIR)

clean:
	rm -rf $(BIN_DEBUG) $(BIN_RELEASE)

# quick inspection helper
tree:
	@command -v tree >/dev/null && tree -a . || find . -maxdepth 3 -print

# Usage:
#   make debug    # builds debug binaries under bin/debug
#   make release  # builds release binaries under bin/release
#   make          # builds both
#   make clean

