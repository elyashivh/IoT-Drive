# Compiler and base flags
CC = g++
BASE_CFLAGS = -pedantic-errors -Wall -Wextra -fPIC
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# Build mode (debug or release)
MODE ?= debug
ifeq ($(MODE),release)
    CFLAGS = $(BASE_CFLAGS) $(RELEASE_FLAGS)
else
    CFLAGS = $(BASE_CFLAGS) $(DEBUG_FLAGS)
endif

# Project paths
F_PROJECT_DIR = $(HOME)/git/projects/final_project
FRAME_WORK_PATH = $(F_PROJECT_DIR)/framework
SRC_DIR = ./src
TEST_DIR = ./test
INC_DIR = ./inc

# Include paths
INC = -I $(INC_DIR) -I $(FRAME_WORK_PATH)/inc

# Library paths
FRAME_WORK_LIB = -L$(FRAME_WORK_PATH) -lframework -Wl,-rpath=$(FRAME_WORK_PATH)
HANDLETON_LIB = -L. -lhandleton_conc -Wl,-rpath=.
HANDLETON_LIB_MIN = -L$(F_PROJECT_DIR)/concrete -lhandleton_conc_min -Wl,-rpath=$(F_PROJECT_DIR)/concrete
FRAMEWORK_HANDLETON_LIB = -L$(FRAME_WORK_PATH) -lhandleton -Wl,-rpath=$(FRAME_WORK_PATH)

# Compiler command
GD = $(CC) $(CFLAGS) $(INC)

# Shared source files (used by both master and minions)
SHARED_SOURCES = $(SRC_DIR)/udp.cpp \
                 $(SRC_DIR)/message.cpp \
                 $(SRC_DIR)/uid.cpp

# Master sources
MASTER_SOURCES = $(SRC_DIR)/response_manager.cpp \
                 $(SRC_DIR)/nbd_proxy.cpp \
                 $(SRC_DIR)/read.cpp \
                 $(SRC_DIR)/write.cpp \
                 $(SRC_DIR)/async_functor.cpp \
                 $(SRC_DIR)/raid_manager.cpp \
                 $(SRC_DIR)/nbd_server.cpp \
                 $(SRC_DIR)/minion_proxy.cpp \
                 $(FRAME_WORK_PATH)/src/reactor.cpp \
                 $(TEST_DIR)/master_test.cpp \
                 $(SHARED_SOURCES)

# Minion sources
MINION_SOURCES = $(SRC_DIR)/minion_concrete.cpp \
                 $(TEST_DIR)/minion_test.cpp \
                 $(SHARED_SOURCES)

# Library sources
LIB_HAND_CONC_SOURCES = $(SRC_DIR)/libhandleton_conc.cpp \
                        $(SRC_DIR)/nbd_server.cpp

LIB_HAND_CONC_MIN_SOURCES = $(SRC_DIR)/libhandleton_conc_min.cpp

# Header dependencies
SHARED_HEADERS = $(INC_DIR)/udp.hpp \
                 $(INC_DIR)/message.hpp \
                 $(INC_DIR)/uid.hpp

MASTER_HEADERS = $(SHARED_HEADERS) \
                 $(INC_DIR)/response_manager.hpp \
                 $(INC_DIR)/nbd_proxy.hpp \
                 $(INC_DIR)/read.hpp \
                 $(INC_DIR)/write.hpp \
                 $(INC_DIR)/async_functor.hpp \
                 $(INC_DIR)/raid_manager.hpp \
                 $(INC_DIR)/nbd_server.hpp \
                 $(INC_DIR)/minion_proxy.hpp

MINION_HEADERS = $(SHARED_HEADERS) \
                 $(INC_DIR)/minion_concrete.hpp

# Framework sources (built in framework directory)
FRAMEWORK_SOURCES = $(FRAME_WORK_PATH)/src/framework.cpp \
                    $(FRAME_WORK_PATH)/src/async_injection.cpp \
                    $(FRAME_WORK_PATH)/src/reactor.cpp \
                    $(FRAME_WORK_PATH)/src/thread_pool.cpp \
                    $(FRAME_WORK_PATH)/src/scheduler.cpp \
                    $(FRAME_WORK_PATH)/src/dir_monitor.cpp \
                    $(FRAME_WORK_PATH)/src/dll_loader.cpp

FRAMEWORK_HANDLETON_SOURCE = $(FRAME_WORK_PATH)/src/libhandleton.cpp

#unit tests
RAID_MNG_SRCS =  $(SRC_DIR)/response_manager.cpp \
                 $(SRC_DIR)/nbd_proxy.cpp \
                 $(SRC_DIR)/read.cpp \
                 $(SRC_DIR)/write.cpp \
                 $(SRC_DIR)/async_functor.cpp \
                 $(SRC_DIR)/raid_manager.cpp \
                 $(SRC_DIR)/nbd_server.cpp \
                 $(SRC_DIR)/minion_proxy.cpp \
                 $(FRAME_WORK_PATH)/src/reactor.cpp \
                 $(TEST_DIR)/raid_manager_test.cpp \
                 $(SHARED_SOURCES)
	
# Executables
EXECUTABLES = master minion1/minion1 minion2/minion2 minion3/minion3
LIBRARIES = libhandleton_conc.so libhandleton_conc_min.so
FRAMEWORK_LIBRARIES = $(FRAME_WORK_PATH)/libframework.so $(FRAME_WORK_PATH)/libhandleton.so

# Default target
.PHONY: all debug release clean help

all: $(FRAMEWORK_LIBRARIES) $(LIBRARIES) $(EXECUTABLES)

debug:
	$(MAKE) all MODE=debug

release:
	$(MAKE) all MODE=release

# Framework libraries (built in framework directory)
$(FRAME_WORK_PATH)/libhandleton.so: $(FRAMEWORK_HANDLETON_SOURCE)
	@echo "Building framework libhandleton.so ($(MODE) mode)..."
	cd $(FRAME_WORK_PATH) && $(GD) -shared $(FRAMEWORK_HANDLETON_SOURCE) -o libhandleton.so

$(FRAME_WORK_PATH)/libframework.so: $(FRAMEWORK_SOURCES) $(FRAMEWORK_HEADERS) $(FRAME_WORK_PATH)/libhandleton.so
	@echo "Building framework libframework.so ($(MODE) mode)..."
	cd $(FRAME_WORK_PATH) && $(GD) -shared -rdynamic $(FRAMEWORK_SOURCES) $(FRAMEWORK_HANDLETON_LIB) -o libframework.so

# Libraries (required for executables)
libhandleton_conc.so: $(LIB_HAND_CONC_SOURCES) $(LIB_HEADERS)
	@echo "Building libhandleton_conc.so ($(MODE) mode)..."
	$(GD) -shared $(LIB_HAND_CONC_SOURCES) -o $@

libhandleton_conc_min.so: $(LIB_HAND_CONC_MIN_SOURCES) $(LIB_HEADERS)
	@echo "Building libhandleton_conc_min.so ($(MODE) mode)..."
	$(GD) -shared $(LIB_HAND_CONC_MIN_SOURCES) -o $@

# Master executable
master: $(FRAME_WORK_PATH)/libframework.so libhandleton_conc.so $(MASTER_SOURCES) $(MASTER_HEADERS)
	@echo "Building master ($(MODE) mode)..."
	$(GD) $(MASTER_SOURCES) $(FRAME_WORK_LIB) $(HANDLETON_LIB) -o $@

# Minion executables
minion1/minion1: $(FRAME_WORK_PATH)/libframework.so libhandleton_conc_min.so $(MINION_SOURCES) $(MINION_HEADERS)
	@echo "Building minion1 ($(MODE) mode)..."
	$(GD) $(MINION_SOURCES) $(FRAME_WORK_LIB) $(HANDLETON_LIB_MIN) \
	-DDISK_STUB=\"./disk_stubs/file1.txt\" -o $@

minion2/minion2: $(FRAME_WORK_PATH)/libframework.so libhandleton_conc_min.so $(MINION_SOURCES) $(MINION_HEADERS)
	@echo "Building minion2 ($(MODE) mode)..."
	$(GD) $(MINION_SOURCES) $(FRAME_WORK_LIB) $(HANDLETON_LIB_MIN) \
	-DDISK_STUB=\"./disk_stubs/file2.txt\" -o $@

minion3/minion3: $(FRAME_WORK_PATH)/libframework.so libhandleton_conc_min.so $(MINION_SOURCES) $(MINION_HEADERS)
	@echo "Building minion3 ($(MODE) mode)..."
	$(GD) $(MINION_SOURCES) $(FRAME_WORK_PATH)/src/thread_pool.cpp \
	$(FRAME_WORK_LIB) $(HANDLETON_LIB_MIN) \
	-DDISK_STUB=\"./disk_stubs/file3.txt\" -o $@

#unit tests
raid_manager: $(RAID_MNG_SRCS)
	$(GD) $(RAID_MNG_SRCS) $(FRAME_WORK_LIB) $(HANDLETON_LIB) -o raid
# Cleanup
clean:
	@echo "Cleaning up..."
	rm -f $(EXECUTABLES) $(LIBRARIES)
	rm -f *.o *.so
	cd $(FRAME_WORK_PATH) && rm -f libframework.so libhandleton.so *.o
	@echo "Clean complete."

# Create necessary directories and stub files
setup:
	@echo "Setting up project structure..."
	mkdir -p $(SRC_DIR) $(TEST_DIR) $(INC_DIR) disk_stubs
	touch disk_stubs/file1.txt disk_stubs/file2.txt disk_stubs/file3.txt
	@echo "Setup complete."

# Framework-only targets (for development)
framework: $(FRAMEWORK_LIBRARIES)
	@echo "Framework libraries built."

run-minions:
	@echo "Starting all minions in separate terminals..."
	gnome-terminal --tab --title="Minion1 (Port 8000)" -- bash -c 'cd $(shell pwd)/minion1 && ./minion1; bash' &
	gnome-terminal --tab --title="Minion2 (Port 8001)" -- bash -c 'cd $(shell pwd)/minion2 && ./minion2; bash' &
	gnome-terminal --tab --title="Minion3 (Port 8002)" -- bash -c 'cd $(shell pwd)/minion3 && ./minion3; bash' &



