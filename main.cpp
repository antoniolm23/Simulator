#include <ctype.h>
#include <unistd.h>
#include <list>
//#include "Node.h"
#include "Stats.h"
#include "strings.h"
#include "Random.h"
#include "Timeslot.h"

#define N 4	//number of slots in slotframe

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
	
	//create list of advertisers
	list<advNode> advNodes;
	
	//list of statistics
	Stat s;
	
	for(int j = 0; j < iterations; j++)
	{
		int advStartingChannel = r.getNumber(advChannels);
		
		//create as many advertisers as needed and then put them in the list
		for(int i = 0; i < advertisers; i++ ) 
		{
			advNode tmp = advNode( (advStartingChannel + i) % advChannels, advChannels );
			advNodes.push_back(tmp);
		}
		
		int listenerChannel = r.getNumber(listChannels);
		
		Timeslot t;
		
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
			
			int frameSlotNumber = 1;
			
			//prints the actual timeslot
			t.print();
			
			/*
			* until both listener and at least one of the advertiser share the same channel
			* the advertisers iterate through channels
			*/
			while(t.compareChannel() == false) 
			{
				frameSlotNumber++;
				t.changeChannel();
				t.print();
			}
			
			//now it's possible to compute statistics
			s.statInsert(frameSlotNumber);
		}
		
		//STAGGERED, as many timeslot in a timeframe as the number of nodes
		if(method == 2)
		{
			int timeslot = 1;
			bool found = false;
			while(found == false) 
			{
				
				for(list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it) 
				{
					t.addNode(*it);
					if(t.compareChannel() == true) 
					{
						found = true;
						s.statInsert(timeslot);
						break;
					}
					it->changeChannel();
				}
				t.erase();
			}
		}
		
		//delete all the lists
		advNodes.erase(advNodes.begin(), advNodes.end());
		t.erase();
	}
	s.print();
	
    return 0;
}
