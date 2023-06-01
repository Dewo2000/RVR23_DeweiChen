CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -lSDL2 -lSDL2_ttf

SOURCES = main.cpp main2.cpp Paddle.cpp Ball.cpp Server.cpp Client.cpp
OBJECTS = Paddle.o Ball.o Client.o
EXECUTABLE = pong server

all: $(SOURCES) $(EXECUTABLE)

pong: $(OBJECTS) main.o Client.o
	$(CC) $(OBJECTS) main.o $(LDFLAGS) -o $@
server: $(OBJECTS) main2.o Server.o
	$(CC) $(OBJECTS) main2.o Server.o $(LDFLAGS) -o $@
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
