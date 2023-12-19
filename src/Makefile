CC = g++
CFLAGS = -std=c++11 -O3 
LIBS = `pkg-config --cflags --libs opencv4` -lncurses

TARGETS = img_ascii

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)