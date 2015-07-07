CFLAGS 	:= -ggdb -Wall -pthread
CC := g++
DEBUG ?= 

ifdef DEBUG
VAR=-DDEBUG
endif

OBJS := Node.o Random.o Stats.o Timeslot.o parameters.o schedule.o

all: simulation

simulation: 		$(OBJS) main.cpp
			$(CC) $(CFLAGS) $(VAR) $^ -o $@ 

Node.o: 		Node.cpp
			g++ -c $(CFLAGS) $(VAR) Node.cpp
Random.o: 		Random.cpp 
			g++ -c $(CFLAGS) $(VAR) Random.cpp
Stats.o:		Stats.cpp
			g++ -c $(CFLAGS) $(VAR) Stats.cpp
Timeslot.o:		Timeslot.cpp
			g++ -c $(CFLAGS) $(VAR) Timeslot.cpp
parameters.o:		parameters.cpp
			g++ -c $(CFLAGS) $(VAR) parameters.cpp
schedule.o:		schedule.cpp
			g++ -c $(CFLAGS) $(VAR) schedule.cpp

			
clean:
	rm *.o
	rm simulation
clean_obj:
	rm *.o
 
