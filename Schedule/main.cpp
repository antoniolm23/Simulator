#include <iostream>
#include <ctype.h>
#include <unistd.h>
#include <list>
#include <stdlib.h>
#include "schedule.h"

using namespace std;

/*
 * This program provides the optimal configuration of a schedule in order of minimizing the 
 * joining time
 */

int main(int argc, char **argv) 
{
	char c;
	int ts = 3;		//# of timeslots
	int nc = 5;		//# of channels
	int na = 3;		//# of advertisers
	double ploss = 0.3; //ploss probability
	/*
	 * this is the parsing of passed arguments:
	 * t is the number of timeslots per slotframe
	 * c is the numer of channels used
	 * a is the number of advertiser cell that can be used
	 */
	while((c = getopt(argc, argv, "t:c:a:p:")) != -1) 
	{
		switch(c)
		{
			case 't':
				ts = atoi(optarg);
				break;
			case 'c':
				nc = atoi(optarg);
				break;
			case 'a':
				na = atoi(optarg);
				break;
			case 'p':
				ploss = atof(optarg);
				break;
			case '?':
				cout<<"Argument not expected\n";
				break;
		}
	}
	
	schedule s(ts, nc, na);
	s.computeSchedule();
	s.setPloss(ploss);
	s.evaluate();
	cout<<s;
	cout<<endl;
	return 0;
}
