CC = gcc
CFLAGS = -Wall -g

all: guesser

cheer.o: cheer.c say.h
	$(CC) $(CFLAGS) -c cheer.c -o cheer.o

grunt.o: grunt.c say.h
	$(CC) $(CFLAGS) -c grunt.c -o grunt.o 

guesser.o: guesser.c say.h
	$(CC) $(CFLAGS) -c guesser.c -o guesser.o

clean:
	rm -f *.o guesser libsay.a 

libsay.a: cheer.o grunt.o
	ar rcs libsay.a cheer.o grunt.o

guesser: guesser.o libsay.a
	$(CC) guesser.o -L. -lsay -o guesser

.PHONY: all clean

