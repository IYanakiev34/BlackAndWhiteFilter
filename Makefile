CC=gcc
CFLAGS= -g -O2 -std=c99 -Wall -pedantic

bmp: main.c
	$(CC) -o $@ $^ $(CFLAGS) 


