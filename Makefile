CC = gcc
CFLAGS = $(shell curl-config --cflags)
LIBS = $(shell curl-config --libs)

all: pull_random

pull_random: pull_random.c
	$(CC) $(CFLAGS) pull_random.c -o pull_random $(LIBS)

clean:
	rm -f pull_random
