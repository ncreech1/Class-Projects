CC = g++

CFLAGS = -g -std=c++11

OBJS = FATRW.o jdisk.o

ALL = ./FATRW

all: $(ALL)

clean:
	rm ./*.o ./FATRW

FATRW: $(OBJS)
	$(CC) $(CFLAGS) -o FATRW $(OBJS)

FATRW.o: FATRW.cpp FATRW.h jdisk.h
	$(CC) $(CFLAGS) -c FATRW.cpp

jdisk.o: jdisk.c jdisk.h
	gcc -c jdisk.c	
