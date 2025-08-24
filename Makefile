CC = gcc
CFLAGS = $(shell curl-config --cflags)
LIBS = $(shell curl-config --libs)

all: pull_random

pull_rand: pull_rand.c
	$(CC) $(CFLAGS) pull_random.c -o pull_random $(LIBS)

clean:
	rm -f pull_random
