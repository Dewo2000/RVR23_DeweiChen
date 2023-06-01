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
OBJECTS = Ball.o Paddle.o
DEPS = Ball.h Paddle.h 

%.o: %.cpp $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: server client

server: $(OBJECTS) main.o
	$(CC) $(OBJECTS) main.o $(LDFLAGS) $(LIBS) -o $@

client: $(OBJECTS) main2.o
	$(CC) $(OBJECTS) main2.o $(LDFLAGS) $(LIBS) -o $@

clean:
	rm -f *.o client server