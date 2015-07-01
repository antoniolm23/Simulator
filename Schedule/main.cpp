#include <iostream>
#include <ctype.h>
#include <unistd.h>
#include <list>
#include <cmath>
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
	int timeslots = 3;		//# of timeslots
	int numberChannels = 5;		//# of channels
	int numberAdvertiser = 3;		//# of advertisers
	int neighbours = 0;
	double ploss = 0.0; //ploss probability
	int energy = 1;
	/*
	 * this is the parsing of passed arguments:
	 * t is the number of timeslots per slotframe
	 * c is the numer of channels used
	 * a is the number of advertiser cell that can be used
	 */
	while((c = getopt(argc, argv, "t:c:a:p:n:e:")) != -1) 
	{
		switch(c)
		{
			case 't':
				timeslots = atoi(optarg);
				break;
			case 'c':
				numberChannels = atoi(optarg);
				break;
			case 'a':
				numberAdvertiser = atoi(optarg);
				break;
			case 'p':
				ploss = atof(optarg);
				break;
			case 'e':
				energy = atoi(optarg);
				break;
			case 'n':
				neighbours = atof(optarg);
				break;
			case '?':
				cout<<"Argument not expected\n";
				break;
		}
	}
	double tmpNeighbours = neighbours;
	double avoidCollision = tmpNeighbours * (1/(tmpNeighbours * energy)) * 
			pow( ((tmpNeighbours - 1)/(tmpNeighbours * energy)), neighbours - 1 ); 
	ploss = 1 - (1 - avoidCollision) * (1 - ploss); 
	cout<<avoidCollision<<endl;
	cout<<ploss<<endl;
	schedule s(timeslots, numberChannels, numberAdvertiser);
	s.computeSchedule();
	s.setPloss(ploss);
	s.evaluate();
	cout<<s;
	cout<<endl;
	return 0;
}
