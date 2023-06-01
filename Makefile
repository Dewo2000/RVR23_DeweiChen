CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -lSDL2 -lSDL2_ttf

SOURCES = main.cpp Paddle.cpp Ball.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = pong

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
