#include "parameters.h"

/**
 * Parses a configuration file and returns the chosen scheduling
 */
map<int, list<int> > parsing::doParsing()
{
	//open the file in read mode
	ifstream file;
	
	file.open(filename.c_str());
	//cout<<filename.c_str()<<endl;
	//temporary strings used to do the parsing
	string tmpNodeID;
	string tmpChOff;
	string tmpTimeslot;
	
	/*
	 * for each line fill a parameter struct, values are separeted by \t
	 * E.g. 1	1	2
	 */
	string line;
	parameter p;
	//retrieve a line from the file
	while(getline(file, line)) 
	{
		//cout<<"doParsing"<<endl;
		std::stringstream iss(line);
		
		//look for parameters separated by \t
		if(getline(iss, tmpNodeID, '\t') && 
			getline(iss, tmpChOff, '\t') &&
			getline(iss, tmpTimeslot, '\t'))
		{
			//if all is right do conversion
			p.nodeID = atoi(tmpNodeID.c_str());
			p.chOff = atoi(tmpChOff.c_str());
			p.timeslot = atoi(tmpTimeslot.c_str());
			//cout<<"chOff: " <<p.chOff<<"\ttimeslot: "<<p.timeslot<<endl;
			schedule[p.timeslot].push_back(p.chOff);
		}
	}
	
	//print the list of obtained parameters to perform a checking
	//for(list<parameter>::iterator at = parameterList.begin(); at != parameterList.end(); ++at)
		//cout<<"NodeID: "<<at->nodeID<<"\tChannelOffset: "<<at->chOff<<"\tTimeslot: "<<at->timeslot<<endl;
	return schedule;
}