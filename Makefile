CXX      := g++
CXXFLAGS := -std=c++17 -O3
LDFLAGS  := `pkg-config --cflags opencv4`
LDLIBS   := -lncurses

SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean asciify

all: asciify

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

# Linking
asciify: $(OBJ)
	@echo "Linking..."
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/asciify $^ $(LDLIBS) `pkg-config --libs opencv4`
	@echo "Done!"

clean:
	rm -r $(OBJ_DIR)
	rm -r $(BIN_DIR)