CC=g++
CFLAGS=-c
std = -std=c++11
AR=ar
ARFLAGS=rcv
OPTFLAGS = -O2
wall= -WALL
all :bin/main
	@echo -n "complete"
bin/main: main.o model.o
		$(CC) $(OPTFLAGS) model.o main.o -o bin/main
main.o : src/main.cpp 
		$(CC) $(std) $(CFLAGS) $< -o $@
model.o : src/model.cpp src/model.h
		$(CC) $(std) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
