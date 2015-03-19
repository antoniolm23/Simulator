CFLAGS 	:= -ggdb -Wall
CC := g++
DEBUG ?= 

OBJS := Node.o Random.o Stats.o Timeslot.o

all: simulation

simulation: 		$(OBJS) main.cpp
			$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ 

Node.o: 		Node.cpp
			g++ -c $(CFLAGS) $(DEBUG) Node.cpp
Random.o: 		Random.cpp 
			g++ -c $(CFLAGS) $(DEBUG) Random.cpp
Stats.o:		Stats.cpp
			g++ -c $(CFLAGS) $(DEBUG) Stats.cpp
Timeslot.o:		Timeslot.cpp
			g++ -c $(CFLAGS) $(DEBUG) Timeslot.cpp

			
clean:
	rm *.o
	rm simulation
clean_obj:
	rm *.o
 
