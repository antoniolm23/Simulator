#include "Stats.h"

/*
 * Insert, if necessary, a statistic referred to a new iteration
 * @params: frequency, timeslotnumber
 */
void Stat::statInsert(int tsn)
{
	//scan the list of statistics and if necessary increment the frequeny
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it) 
	{
		if(it->timeslotNumber == tsn) 
		{
			it->frequency++;
			return;
		}
	}
	
	//if the scan wasn't successful add a new record in the list
	statElem st;
	st.frequency = 1;
	st.timeslotNumber = tsn;
	statsList.push_back(st);
}


//prints the list of statistics on file
void Stat::print()
{
	ofstream myfile;
	myfile.open("statistics.txt", ios::app);
	
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		myfile<<"timeslotNumber\t"<<it->timeslotNumber<<"\tfrequency:\t"<<it->frequency<<endl; 
	}
	myfile.close();
}



