CC = gcc
CFLAGS = --std=c99 -g -Werror -Wall -pedantic
LDFLAGS = -lGL -lGLU -lglut

all: main

main: main.c asteroid.o draw.o player.o alien.o bullet.o linked_list.o interactions.o game.o explosion.o
	$(CC) $(CFLAGS) $(LDFLAGS) $+ -o main

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c -o $*.o

clean:
	-rm *.o
	-rm main
