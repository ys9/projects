CC=gcc
#adjust APUE dir root accordingly
APUE=$(HOME)/222/apue.3e
INC_PATH=-I$(APUE)/include
LIB_PATH=-L$(APUE)/lib
LIB=-lapue

all: bmp

bmp: bmp.c
	$(CC) $(CFLAGS) $(INC_PATH) $(LIB_PATH) $@.c $(LIB) -o $@ -lm

clean:
	rm bmp
