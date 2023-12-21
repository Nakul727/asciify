CC = g++
CFLAGS = -std=c++17 -O3 
LIBS = `pkg-config --cflags --libs opencv4` -lncurses
SRC = ./src
OBJ = ./obj
BIN = ./bin

.PHONY: all clean asciify

all: asciify

$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBS)

asciify: $(OBJ)/main.o $(OBJ)/asciify.o
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/asciify $^ $(LIBS) 

clean:
	rm -r $(OBJ)
	rm -r $(BIN)