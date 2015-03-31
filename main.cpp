#include <ctype.h>
#include <unistd.h>
#include <list>
//#include "Node.h"
#include "Stats.h"
#include "strings.h"
//#include "Random.h"
#include "Timeslot.h"
#include "parameters.h"

/*
 * parameters to be passed to main:
 * -i number of iterations, predefined 10000
 * -a number of advertisers, predefined 1
 * -c channel of advertiser, predefined 16
 * -l channel of listener, predefined 16
 * -m method, predefined fixed
 * -s schema used e.g. A0C1 
 */

int main(int argc, char **argv) 
{
	
	int c;
	
	//parameters to be set with predefined values
	int iterations = 10 * 1000;
	int advertisers = 1;
	int advChannels = 16;
	int listChannels = 16;
	int method = 1;
	string sTmp = "";
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:a:c:l:m:s:")) != -1) 
	{
		switch(c)
		{
			case 'i':
				iterations = atoi(optarg);
				break;
			case 'a':
				advertisers = atoi(optarg);
				break;
			case 'c':
				advChannels = atoi(optarg);
				break;
			case 'l':
				listChannels = atoi(optarg);
				break;
			case 'm':
				method = atoi(optarg);
				break;
			case 's':
				sTmp = optarg;
				break;
			case '?':
				cout<<optarg<<endl;
				break;
		}
	}
	
	if(listChannels > advChannels)
		listChannels = advChannels;
	//cout<<advChannels<<endl;
	
	//create a general class from which random numbers will be extracted
	Random r;
	r.init();
	
	//create list of advertisers
	list<advNode> advNodes;
	
	//list of statistics
	Stat s;
	/***Parameter setting***/
	string filename = "conf.txt";
	parsing p = parsing(filename);
	p.doParsing();
	
	int id = -1;
	
	advNode tmp = advNode(advChannels); 
	
	//create as many advertisers as needed and then put them in the list
	while(p.getParameterSize() > 0) 
	{
		//cout<<"parameter size: "<<p.getParameterSize()<<"\n";
		//get a new parameter
		parameter pa = p.getParameter();
		
		//if I'm inserting a new link for a given node
		if(id == pa.nodeID) 
		{
			tmp.insertLink(pa.chOff, pa.timeslot);
		}
		
		//otherwise insantiate a new node
		else
		{
			if(id != -1) 
			{
				advNodes.push_back(tmp);
				//cout<<"node inserted\n";
			}
			tmp = advNode(advChannels);
			tmp.setNodeID(pa.nodeID);
			tmp.insertLink(pa.chOff, pa.timeslot);
			id = pa.nodeID;
		}
		
	}
	
	advNodes.push_back(tmp);
		
	for(int j = 0; j < iterations; j++)
	{
		
		int listenerChannel = r.getNumber(listChannels);
		
		Timeslot t = Timeslot(r, listChannels);
		
		//add the listener
		t.addListener(listenerChannel);
		
		//build the list of advertisers
		for( list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it  )
		{
			t.addNode(*it);
		}
		
		int frameSlotNumber = t.timeslotManager(method);
		
		//now it's possible to compute statistics
		s.statInsert(frameSlotNumber);
		
		//delete all the lists
		t.erase();
	}
	advNodes.erase(advNodes.begin(), advNodes.end());
	s.print(sTmp);
	
    return 0;
}
