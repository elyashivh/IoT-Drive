# set FN=program_name
FN ?= foo
SRC = src/$(FN).cpp

UPPER_FN = $(shell echo $(FN) | tr 'a-z' 'A-Z')


# output
DEBUG = bin/debug
FULL_DEBUG = /home/elyashiv/git/projects/final_project/framework/bin/debug
RELEASE = bin/release

TARGET = $(DEBUG)/$(FN).out

# header
HEADER_DIR = include
HEADER = include/$(FN).hpp

# srcs
SRC_DIR = src

# template implementation file (for Handleton)
TEMPLATE_IMPLS = src/Handleton.cpp
TEMPLATE_OBJS_DEBUG = $(patsubst src/%.cpp,$(DEBUG)/%.o,$(TEMPLATE_IMPLS))
TEMPLATE_OBJS_RELEASE = $(patsubst src/%.cpp,$(RELEASE)/%.o,$(TEMPLATE_IMPLS))

# tests and test objects
TESTS = test/$(FN)_test.cpp
TEST_OBJS = $(TESTS:.cpp=.o)

DEPENDENCIES1 = $(shell g++ -I$(HEADER_DIR) -MM $(SRC) $(TESTS))
EXTRACTED_SRCS1 = $(patsubst $(HEADER_DIR)/%.hpp,$(SRC_DIR)/%.cpp,$(filter %.hpp, $(DEPENDENCIES1)))
SRCS_FILES1 = $(foreach src,$(EXTRACTED_SRCS1),$(if $(wildcard $(src)), $(src)))

DEPENDENCIES = $(shell g++ -I$(HEADER_DIR) -MM $(SRCS_FILES1))
EXTRACTED_SRCS = $(patsubst $(HEADER_DIR)/%.hpp,$(SRC_DIR)/%.cpp,$(filter %.hpp, $(DEPENDENCIES)))
#SRCS_FILES = $(foreach src,$(EXTRACTED_SRCS),$(if $(wildcard $(src)), $(src)))
#SRCS_FILES = $(sort $(foreach src,$(EXTRACTED_SRCS),$(if $(wildcard $(src)),$(src))))

# Remove duplicates while preserving order
uniq = $(strip $(eval _seen :=) $(foreach _v,$1,$(if $(filter $(_v),$(_seen)),,$(eval _seen += $(_v)) $(_v))))
SRCS_FILES_RAW = $(foreach src,$(EXTRACTED_SRCS),$(if $(wildcard $(src)),$(src)))
SRCS_FILES = $(call uniq,$(SRCS_FILES_RAW))


$(info $(SRCS_FILES))

# dependencies 
DEPS_DIR = deps
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(DEPS_DIR)/%.d,$(SRCS_FILES))

# libraries
LIBS = $(patsubst src/%.cpp,-l%,$(SRCS_FILES))

# src objects
DEBUG_OBJS = $(patsubst src/%.cpp,$(DEBUG)/%.o,$(SRCS_FILES))
RELEASE_OBJS = $(patsubst src/%.cpp,$(RELEASE)/%.o,$(SRCS_FILES))

# shared objects
DEBUG_SO = $(patsubst src/%.cpp,$(DEBUG)/lib%.so,$(SRCS_FILES))
RELEASE_SO = $(patsubst src/%.cpp,$(RELEASE)/lib%.so,$(SRCS_FILES))

# preprocessor and compiler flags
CPPFLAGS = -I$(HEADER_DIR)
CC = g++ -std=c++11
PIC = -fPIC

# debug flags 
#ANSI = -ansi #DISABLED FOR C++11
#GD = -pedantic-errors -Wall -Wextra -pthread -fsanitize=thread -g
GD = -pedantic-errors -Wall -Wextra -pthread -g
GC = -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS = $(GD)

# linker flags
#LDFLAGS = -fsanitize=thread -L$(DEBUG) -Wl,-rpath,$(FULL_DEBUG)
LDFLAGS = -L$(DEBUG) -Wl,-rpath,$(FULL_DEBUG)
LDSHARED = -shared


.PHONY: open create deps debug release test clean

default: deps debug test clean

all: debug release clean

deps: $(DEP)
-include $(DEP)

debug: $(DEBUG_OBJS) $(DEBUG_SO) $(TEMPLATE_OBJS_DEBUG)

release: CFLAGS = $(GC)
release: $(RELEASE_OBJS) $(RELEASE_SO) $(TEMPLATE_OBJS_RELEASE)

test: $(TEST_OBJS) $(TARGET)

clean:
	rm -rf $(DEBUG_OBJS) $(RELEASE_OBJS) $(TEST_OBJS) $(DEP) $(TEMPLATE_OBJS_DEBUG) $(TEMPLATE_OBJS_RELEASE)

# open all project files in gedit
open: 
	gedit $(HEADER) $(SRC) $(TESTS) & 

# init project files
create: $(SRC) $(TESTS) $(HEADER)

$(SRC) $(TESTS) $(HEADER): 
	touch $(SRC)
	echo "#include \"$(FN).hpp\"" >> $(SRC)
	touch $(TESTS)
	echo "#include \"$(FN).hpp\"" >> $(TESTS)
	touch $(HEADER)
	echo "#ifndef __ILRD_$(UPPER_FN)_HPP__\n#define __ILRD_$(UPPER_FN)_HPP__\n\n\n#endif" >> $(HEADER)

# create dependencies files
$(DEP): deps/%.d: src/%.cpp
	$(CC) -MM $(CPPFLAGS) $< > $@

# create .o files for normal sources
$(DEBUG_OBJS): $(DEBUG)/%.o: src/%.cpp
	$(CC) $(PIC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(RELEASE_OBJS): $(RELEASE)/%.o: src/%.cpp
	$(CC) $(PIC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# test object
$(TEST_OBJS): $(TESTS) $(HEADER)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# create .so
$(DEBUG_SO): $(DEBUG)/lib%.so: $(DEBUG)/%.o
	$(CC) $(LDSHARED) $< -o $@

$(RELEASE_SO): $(RELEASE)/lib%.so: $(RELEASE)/%.o
	$(CC) $(LDSHARED) $< -o $@

# create executable
$(TARGET): $(TEST_OBJS) 
	$(CC) $^ -o $@ $(LDFLAGS) $(LIBS)
