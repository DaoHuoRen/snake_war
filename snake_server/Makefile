CC = g++
CFLAGS = -std=c++11
 
all: main.cpp server.o map.o food.o snake.o
	$(CC) $(CFLAGS) main.cpp  server.o  map.o food.o snake.o -o snake_server -lpthread

server.o: server.cpp server.h config.h map.h snake.h
	$(CC) $(CFLAGS) -c server.cpp

food.o: food.cpp food.h config.h
	$(CC) $(CFLAGS) -c food.cpp

map.o: map.cpp map.h config.h food.h snake.h
	$(CC) $(CFLAGS) -c map.cpp

snake.o: snake.cpp snake.h map.h config.h
	$(CC) $(CFLAGS) -c snake.cpp

clean:
	rm -f *.o snake_server

