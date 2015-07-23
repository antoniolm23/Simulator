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
	int neighbours = 1;
	double ploss = 0.0; //ploss probability
	int energy = 1;
	bool file = false;
	/*
	 * this is the parsing of passed arguments:
	 * t is the number of timeslots per slotframe
	 * c is the numer of channels used
	 * a is the number of advertiser cell that can be used
	 * f is the file with the optimal distances to have, the file is fixed and is "distances.txt"
	 */
	while((c = getopt(argc, argv, "t:c:a:p:n:e:f")) != -1) 
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
			case 'f':
				file = true;
				break;
			case '?':
				cout<<"Argument not expected\n";
				break;
		}
	}
	double tmpNeighbours = neighbours;
	double correctTransmission = tmpNeighbours * (1/ (tmpNeighbours * energy)) * 
			pow( ((tmpNeighbours * energy - 1 )/(tmpNeighbours * energy)), neighbours - 1 ); 
	ploss = 1 - (correctTransmission) * (1 - ploss); 
	cout<<correctTransmission<<endl;
	cout<<ploss<<endl;
	if(file == false)
	{
		schedule s(timeslots, numberChannels, numberAdvertiser);
		s.computeSchedule();
		s.setPloss(ploss);
		s.evaluate();
		cout<<s;
		cout<<endl;
	}
	
	/**
	 * we know which are the distances to use, so the only thing to do is to gather
	 * timeslot and channel offset
	 */ 
	else
	{
		schedule s("conf.txt", timeslots, numberChannels);
		s.retrieveSchedule();
	}
	return 0;
}
