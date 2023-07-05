CC = g++
CFLAGS = -std=c++11

all: juego

juego: main.cpp
	$(CC) $(CFLAGS) main.cpp -o juego

clean:
	rm -f juego