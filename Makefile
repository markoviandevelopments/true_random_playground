CC = gcc
CFLAGS = $(shell curl-config --cflags)
LIBS = $(shell curl-config --libs)

all: pull_rand

pull_rand: pull_rand.c
	$(CC) $(CFLAGS) pull_rand.c -o pull_rand $(LIBS)

clean:
	rm -f pull_rand
