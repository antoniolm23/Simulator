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
	int iterations = 100 * 1000;
	int advChannels = 16;
	int listChannels = 16;
	int method = 1;
	string sTmp = "";
	double ploss = 0.0;
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:c:l:m:s:p:")) != -1) 
	{
		switch(c)
		{
			case 'i':
				iterations = atoi(optarg);
				break;
			/*
			 * Advertisers removed since I use a file that states the number of advertisers
			case 'a':
				advertisers = atoi(optarg);
				break;
			*/
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
			case 'p':
				/*
				 * ploss is inserted as integer.floating
				 * e.g. 30% => 0.3
				 */
				ploss = atof(optarg);
				//cout<<optarg<<": "<<ploss<<"\n";
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
		
		//get the number of listener channel
		int listenerChannel = r.getNumber(listChannels);
		
		Timeslot t = Timeslot(r, listChannels);
		
		//check if we need to add the probability
		if(method == PLOSS_SCENARIO)
			t.setProbability(ploss);
		
		//add the listener
		t.addListener(listenerChannel);
		
		//build the list of advertisers
		for( list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it  )
		{
			t.addNode(*it);
		}
		
		/*
		 * via the timeslot manager gather:
		 * in the fixed schema the number of framslot elapsed
		 * in the others the number of timeslots elapsed
		 */
		int slotNumber = t.timeslotManager(method);
		
		//now it's possible to compute statistics
		s.statInsert(slotNumber);
		
		//delete all the lists
		t.erase();
		char f;
		//cin>>f;
		//cout<<f<<endl;
	}
	//free resources
	advNodes.erase(advNodes.begin(), advNodes.end());
	
	//compute and print statistics
	s.setIterations(iterations);
	s.print(sTmp);
	
    return 0;
}
