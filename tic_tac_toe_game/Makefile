CC=gcc
CFLAGS=-Wall -g
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lm

all: tic_tac_toe.o ttt

tic_tac_toe.o: tic_tac_toe.c tic_tac_toe.h
	$(CC) $(CFLAGS) -c tic_tac_toe.c

ttt: ttt.c tic_tac_toe.o
	$(CC) $(CFLAGS) $(SDL_CFLAGS) ttt.c sdl.o tic_tac_toe.o -o tic_tac_toe $(SDL_LDFLAGS)

clean:
	rm tic_tac_toe tic_tac_toe.o
