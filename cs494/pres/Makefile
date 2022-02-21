CC = g++

CFLAGS = -std=c++11

OBJS = cutslimes.o

ALL = ./CutSlimes

all: $(ALL)

clean: 
	rm ./*.o ./CutSlimes

CutSlimes: $(OBJS)
	$(CC) $(CFLAGS) -o CutSlimes $(OBJS)

cutslimes.o: cutslimes.cpp
	$(CC) $(CFLAGS) -c cutslimes.cpp
	
