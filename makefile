CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++14

all: assembler-menu.o assembler.o manchester.o
	$(CC) $(CFLAGS) -o assemble assembler-menu.o assembler.o
	$(CC) $(CFLAGS) -o baby manchester.o

assembler-menu.o: assembler-menu.cpp assembler.h
	$(CC) $(CFLAGS) -c assembler-menu.cpp

assembler.o: assembler.cpp assembler.h
	$(CC) $(CFLAGS) -c assembler.cpp

manchester.o: manchester.cpp
	$(CC) $(CFLAGS) -c manchester.cpp

clean:
	rm -f assemble baby *.o

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<
