#include <ctype.h>
#include <unistd.h>
#include <list>
//#include "Node.h"
#include "Stats.h"
#include "strings.h"
//#include "Random.h"
#include "Timeslot.h"

/*
 * parameters to be passed to main:
 * -i number of iterations, predefined 10000
 * -a number of advertisers, predefined 1
 * -c channel of advertiser, predefined 16
 * -l channel of listener, predefined 16
 * -m method, predefined fixed
 */

int main(int argc, char **argv) 
{
	
	int c;
	
	//parameters to be set with predefined values
	int iterations = 100 * 1000;
	int advertisers = 1;
	int advChannels = 16;
	int listChannels = 16;
	int method = 1;
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:a:c:l:m:")) != -1) 
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
			case '?':
				cout<<optarg<<endl;
				break;
		}
	}
	
	//create a general class from which random numbers will be extracted
	Random r;
	r.init();
	
	//create list of advertisers
	list<advNode> advNodes;
	
	//list of statistics
	Stat s;
	
	for(int j = 0; j < iterations; j++)
	{
		
		/*
		 * FIXME FOR NOW static link equal for all required nodes
		 */
		int channelOffset = 0;
		int timeslot = 0;
		
		//create as many advertisers as needed and then put them in the list
		for(int i = 0; i < advertisers; i++ ) 
		{
			advNode tmp = advNode( advChannels );
			tmp.insertLink(channelOffset, timeslot);
			advNodes.push_back(tmp);
		}
		
		int listenerChannel = r.getNumber(listChannels);
		
		Timeslot t = Timeslot(r);
		
		//add the listener
		t.addListener(listenerChannel);
		
		/*
		* METHOD FIXED: First Timeslot of each Frameslot with all nodes
		*/
		if(method == 1)
		{
			//build the list of advertisers
			for( list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it  )
			{
				t.addNode(*it);
			}
			
			int frameSlotNumber = t.timeslotManager(method);
			
			//now it's possible to compute statistics
			s.statInsert(frameSlotNumber);
		}
		
		//delete all the lists
		advNodes.erase(advNodes.begin(), advNodes.end());
		t.erase();
	}
	s.print();
	
    return 0;
}
