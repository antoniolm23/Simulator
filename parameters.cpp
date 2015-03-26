#include "parameters.h"

void parsing::doParsing()
{
	//open the file in read mode
	ifstream file;
	file.open(filename.c_str());
	
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
			
			//insert the retrieved parameter in the list
			parameterList.push_back(p);
		}
	}
	
	//print the list of obtained parameters to perform a checking
	//for(list<parameter>::iterator at = parameterList.begin(); at != parameterList.end(); ++at)
		//cout<<"NodeID: "<<at->nodeID<<"\tChannelOffset: "<<at->chOff<<"\tTimeslot: "<<at->timeslot<<endl;
	
}

/*
 * retrieve the element at the top of the list
 * @return struct containing the parameter used
 */
parameter parsing::getParameter()
{
	parameter p;
	p = parameterList.front();
	
	parameterList.pop_front();
	
	return p;
}

int parsing::getParameterSize()
{
	return parameterList.size();
}
