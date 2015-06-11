#include <list>
#include <iostream>
#include "define.h"
using namespace std;


class Schedule
{
	list<advLink> scheduling;
	list<int> ti;		//list of the various ti at which a beacon is transmitted
	int totChannel;		//amount of channels that can be used
	int totSlots;		//amount of timeslots per slotframe
	int totAdvertiser;	//amount f advertising cells that can be used
	friend ostream& operator<<(ostream&, const Schedule& );
	double ploss;
	
	int computeTimeslot(int);
	int computeChannelOffset(int);
	int computePerformance(int, int, int);
public:
	Schedule(int, int, int);
	list<advLink> computeSchedule();
	void evaluate();
	void setPloss(double);
};