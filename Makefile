# CC = g++
# CFLAGS = -std=c++17 -Wall -Wextra -pedantic
# LDFLAGS = -lSDL2 -lSDL2_ttf

# SOURCES = main.cpp Paddle.cpp Ball.cpp
# OBJECTS = $(SOURCES:.cpp=.o)
# EXECUTABLE = pong

# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# .cpp.o:
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJECTS) $(EXECUTABLE)

CC = g++
CFLAGS = -g -I. -std=c++11
LDFLAGS = -lSDL2 -lSDL2_ttf
LIBS=-lpthread
SOURCES = main.cpp Paddle.cpp Ball.cpp 
OBJECTS = Ball.o main.o Paddle.o Client.o Socket.o
DEPS = Socket.h Ball.h Client.h Paddle.h Server.h

%.o: %.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: pong server

pong: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

server: Server.o Paddle.o Socket.o
	$(CC) Server.o Paddle.o Socket.o $(LDFLAGS) $(LIBS) -o $@


clean:
	rm -f *.o pong server