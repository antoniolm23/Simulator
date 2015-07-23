#include <list>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
/*
 * This is the schedule class used to compute the optimal 
 * scheduling algorithm
 */

struct timeslot
{
	int nc;		//channeloffset used
	int ts;		//timeslot used
};

class schedule
{
	list<timeslot> scheduling;
	list<int> ti;		//list of the various ti at which a beacon is transmitted
	int totChannel;		//amount of channels that can be used
	int totSlots;		//amount of timeslots per slotframe
	int totAdvertiser;	//amount f advertising cells that can be used
	friend ostream& operator<<(ostream&, const schedule& );
	double ploss;
	
	string filename;
	int computeTimeslot(int);
	int computeChannelOffset(int);
	int computePerformance(int, int, int);
	void saveOnFile(int, int);
public:
	schedule(int, int, int);
	schedule(string, int, int);
	void computeSchedule();
	void evaluate();
	void setPloss(double);
	void retrieveSchedule();
};