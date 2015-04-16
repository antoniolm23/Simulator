CFLAGS 	:= -ggdb -Wall
CC := g++
DEBUG ?= 

OBJS := schedule.o 

all: schedules

schedules: 		$(OBJS) main.cpp
			$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ 

schedule.o: 		schedule.cpp
			g++ -c $(CFLAGS) $(DEBUG) schedule.cpp
			
clean:
	rm *.o
	rm schedules
clean_obj:
	rm *.o
 
