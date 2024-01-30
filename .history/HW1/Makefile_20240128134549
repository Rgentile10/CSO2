CC = gcc
CFLAGS = -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic
LDFLAGS = -fsanitize=address

all: split

split: split.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

split.o: split.c split.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c split.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f split split.o main.o

.PHONY: all clean