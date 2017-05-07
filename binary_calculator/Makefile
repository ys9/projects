CC=gcc
CFLAGS=-g -Wall

all: binary_calculator binary_calculator_main

binary_calculator: binary_calculator.c binary_calculator.h
	$(CC) $(CFLAGS) -c binary_calculator.c

binary_calculator_main: binary_calculator_main.c binary_calculator.o
	$(CC) $(CFLAGS) binary_calculator.o binary_calculator_main.c -o binary_calculator -lm

clean:
	rm binary_calculator binary_calculator.o binary_calculator_main
