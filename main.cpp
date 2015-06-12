#include <ctype.h>
#include <unistd.h>
#include <list>
//#include "Node.h"
#include "Stats.h"
#include "strings.h"
//#include "Random.h"
#include "Timeslot.h"
#include "parameters.h"
#include "schedule.h"

bool search(position p, list<advNode> listAdv)
{
	for(list<advNode>::iterator it = listAdv.begin(); it != listAdv.end(); ++it)
	{
		if(p.x == it -> getPosX() && p.y == it -> getPosY())
			return true;
	}
	return false;
}

//utility function to generate positions of advertiser nodes
position generatePosition(int squareSide, Random random, list<advNode> listAdv)
{
	
	bool acceptable = true;
	position p;
	while(acceptable)
	{
		p.x = random.getNumber(squareSide);
		p.y = random.getNumber(squareSide);
		acceptable = search(p, listAdv);
	}
	return p;
}

//writes the generated topology on a file
void saveTopology(list<advNode> advNodes)
{
	ofstream file;
	file.open("topology.txt", ios::app);
	file << "****************\n";
	for(list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it)
	{
		file << it -> getPosX() << '\t' << it -> getPosY() << '\n';
	}
}

/*
 * parameters to be passed to main:
 * -i number of iterations for each listener position, predefined 10000
 * -a number of advertisers, predefined 1
 * -c channel of advertiser, predefined 16
 * -l channel of listener, predefined 16
 * -s schema used e.g. A0C1 
 * -p ploss probability
 * -t transmission range
 * -q square side
 * -o # of topologies to generate per # of schema
 * -y synchronization
 * -f configurationFile in order to enable the possibility of using a configuration file too
 * -b advertising cells to be used
 * -z possible listener positions
 */
int main(int argc, char **argv) 
{
	
	int c;
	
	//parameters to be set with predefined values
	int iterations = 10 * 1000;
	int advertiserChannels = 16;
	int listenerChannels = 16;
	int transmissionRange = 20;
	int squareSide = 20;
	int topologiesToGenerate = 50;
	int advertisers = 10;
	bool advertiserSynchronized = true;
	bool configurationFile = false;
	int numberAdvertisingCells = 3;
	string nameOfSchema = "";
	double ploss = 0.0;
	int numberListenerPositions = 10;
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:a:c:l:s:p:t:q:o:y:f:b:z:")) != -1) 
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
				advertiserChannels = atoi(optarg);
				break;
			case 'l':
				listenerChannels = atoi(optarg);
				break;
			case 's':
				nameOfSchema = optarg;
				break;
			case 'p':
				/*
				 * ploss is inserted as integer.floating
				 * e.g. 30% => 0.3
				 */
				ploss = atof(optarg);
				//cout<<optarg<<": "<<ploss<<"\n";
				break;
			case 't':
				transmissionRange = atoi(optarg);
				break;
			case 'q':
				squareSide = atoi(optarg);
				break;
			case 'o':
				topologiesToGenerate = atoi(optarg);
				break;
			case 'y':
				advertiserSynchronized = atoi(optarg);
				break;
			case 'f':
				configurationFile = atoi(optarg);
				break;
			case 'b':
				numberAdvertisingCells = atoi(optarg);
				break;
			case 'z':
				numberListenerPositions = atoi(optarg);
				break;
			case '?':
				cout<<optarg<<endl;
				break;
		}
	}
	
	if(listenerChannels > advertiserChannels)
		listenerChannels = advertiserChannels;
	//cout<<N<<endl;
	
	//create a general class from which random numbers will be extracted
	Random random;
	random.init();
	
	//create list of advertisers
	list<advNode> advNodes;
	
	//list of statistics
	Stat s;
	Timeslot timeslot = Timeslot(random, transmissionRange, listenerChannels);
	listenerNode listener;
	
	//BEGIN comments
// 	if(configurationFile)
// 	{
// 		/***Parameter setting***/
// 		string filename = "conf.txt";
// 		parsing p = parsing(filename);
// 		p.doParsing();
// 		
// 		int id = -1;
// 		
// 		advNode tmp = advNode(advertiserChannels); 
// 		
// 		//create as many advertisers as needed and then put them in the list
// 		while(p.getParameterSize() > 0) 
// 		{
// 			//cout<<"parameter size: "<<p.getParameterSize()<<"\n";
// 			//get a new parameter
// 			parameter pa = p.getParameter();
// 			
// 			//if I'm inserting a new link for a given node
// 			if(id == pa.nodeID) 
// 			{
// 				tmp.insertLink(pa.chOff, pa.timeslot);
// 			}
// 			
// 			//otherwise insantiate a new node
// 			else
// 			{
// 				if(id != -1) 
// 				{
// 					advNodes.push_back(tmp);
// 					//cout<<"node inserted\n";
// 				}
// 				tmp = advNode(advertiserChannels);
// 				tmp.setNodeID(pa.nodeID);
// 				tmp.insertLink(pa.chOff, pa.timeslot);
// 				id = pa.nodeID;
// 			}
// 			
// 		}
// 		
// 		advNodes.push_back(tmp);
// 		
// 		//Timeslot characteristics remain equal through all the iterations
// 		
// 		//build the list of advertisers
// 		for( list<advNode>::iterator it = advNodes.begin(); it != advNodes.end(); ++it  )
// 		{
// 			timeslot.addNode(*it);
// 		}
// 	}
	//END comments
	
	/**
	 * If a configuration file is not used the program has to perform many things:
	 * a) generate the optimal schema
	 * b) generate nodes positions
	 * c) generate listener position
	 */
	/*** a ***/
	/*compute the list of advertising cells to be used*/
	list<advLink> advertisingCells = list<advLink>();
	Schedule schedule = Schedule(N, advertiserChannels, numberAdvertisingCells);
	advertisingCells = schedule.computeSchedule();
	//cout << schedule << endl;
	for(int j = 0; j < topologiesToGenerate; j++)
	{
		cout << "step: " << j << endl;
		/*** b ***/
		//generate advertisers
		for(int i = 0; i < advertisers; i++)
		{
			//set available channels
			advNode node = advNode(advertiserChannels, transmissionRange);
			
			//insert link
			node.insertLinks(advertisingCells);
			
			//generate node position, check if the position is already occupied
			position p = generatePosition(squareSide + 1, random, advNodes);
			node.setPosition(p);

			cout << "Positions: " << p.x << "\t" << p.y << endl;
			
			//set type of node
			if(i == 0)
				node.setType(COORDINATOR);
			else
			{
				node.setType(ADVERTISER);
				node.setSynchronization(advertiserSynchronized);
			}
			
			//set the id of the node
			int id = i + 1;
			node.setNodeID(id);
			
			//insert nodes in lists
			advNodes.push_back(node);
			timeslot.addNode(node);
		}
		saveTopology(advNodes);
		/*** c ***/
		for(int lp = 0; lp < numberListenerPositions; lp++)
		{
			//generate the listener
			bool acceptable = false;
			while(!acceptable) 
			{
				//set listener properties excep its channel 
				position p = generatePosition(squareSide, random, advNodes);
				listener.xPos = p.x;
				listener.yPos = p.y;
				listener.channelUsed = 0;
				acceptable = timeslot.setListener(listener);
			}
			
			cout<<"listener: "<< listener.xPos << "\t" << listener.yPos << endl;
			
			//timeslot.addListener(listener);
			for(int c = 0; c < iterations; c++)
			{
				//check if we need to add the probability
				if(ploss != 0)
					timeslot.setProbability(ploss);
				
				//set the listener channel
				int channel = random.getNumber(listenerChannels);
				timeslot.setListenerChannel(channel);
				
				/*
				* via the timeslot manager gather:
				* in the fixed schema the number of framslot elapsed
				* in the others the number of timeslots elapsed
				*/
				int slotNumber;
				if(ploss != 0)
					slotNumber = timeslot.timeslotManager(PLOSS_SCENARIO);
				else
					slotNumber = timeslot.timeslotManager(STAGGERED);
				
				//now it's possible to compute statistics
				s.statInsert(slotNumber);
				
			}
			
		}
		//free resources before generating a new topology with the same density
		timeslot.erase();
		advNodes.erase(advNodes.begin(), advNodes.end());
	}
	
	//compute and print statistics
	s.setIterations(iterations);
	s.print(nameOfSchema);
	
    return 0;
}
