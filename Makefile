CXX=g++
SRC_DIR=src
BUILD_DIR=build
LIB_DIR=lib
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror -pedantic -g
LD_FLAGS=

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Raylib
RAYLIB_DIR=$(LIB_DIR)/raylib
CXXFLAGS+=-I$(RAYLIB_DIR)/src
LD_FLAGS+=-L$(RAYLIB_DIR)/src -lraylib -L/opt/homebrew/lib/ -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit


build: $(BUILD_DIR)/main

$(BUILD_DIR)/main: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -rf $(BUILD_DIR)