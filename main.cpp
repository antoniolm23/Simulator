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
#include "define.h"
#include <sstream>

/**
 * Given cardinal position checks if two motes are or not neighbours
 * basing on the transmissionRange
 * @param: cardinal position of node a and of node b, the transmission range
 * @return: possible constant: OCCUPIED, INTXRANGE, AVAILABLEPOS 
 */
/*int checkNeighbours(double aPosX, double aPosY, double bPosX, double bPosY, int transmissionRange)
{
	double dX = (aPosX - bPosX) * (aPosX - bPosX);
	double dY = (aPosY - bPosY) * ( aPosY - bPosY);
	double distance = sqrt((dX) + dY );
	//cout<<distance<<endl;
	if(distance < transmissionRange && distance != 0)
		return INTXRANGE;
	if(distance != 0)
		return AVAILABLEPOS;
	if(distance == 0)
		return OCCUPIED;
	return ERROR;
}*/

/**
 * Convert an integer to string
 * @param: t is the integer to convert
 * @return: the resultant string 
 */
string convertInt(int t)
{
	stringstream convert;	//needed to convert double to string
	string tmpString;
	convert << t;
	tmpString = convert.str();
	return tmpString;
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
 * -S square side
 * -T # of topologies to generate per # of schema
 * -y synchronization
 * -f fair method
 * -C advertising cells to be used
 * -P possible listener positions
 * -E energy factor
 * -F configurationFile
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
	int topologiesToGenerate = 1;
	int numberAdvertisers = 10;
	bool advertiserSynchronized = true;
	int numberAdvertisingCells = 0;
	string nameOfSchema = "";
	string fileName = "";
	double ploss = 0.0;
	int numberListenerPositions = 10;
	double energyFactor = 1;
	int fair = 0;
	bool configurationFile = false; 
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:a:c:l:s:p:t:S:T:y:C:P:E:f:F:")) != -1) 
	{
		switch(c)
		{
			case 'i':
				iterations = atoi(optarg);
				break;
			case 'a':
				numberAdvertisers = atoi(optarg);
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
			case 'S':
				squareSide = atoi(optarg);
				break;
			case 'T':
				topologiesToGenerate = atoi(optarg);
				break;
			case 'y':
				advertiserSynchronized = atoi(optarg);
				break;
			case 'C':
				numberAdvertisingCells = atoi(optarg);
				cout<<numberAdvertisingCells<<endl;
				break;
			case 'P':
				numberListenerPositions = atoi(optarg);
				break;
			case 'E':
				energyFactor = atof(optarg);
				break;
			case 'f':
				fair = atoi(optarg);
				break;
			case 'F':
				configurationFile = true;
				fileName = optarg;
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
	list<advNode> tmpAdvNodes = list<advNode>();
	
	//needed objects
	Stat statistics;
	
	/**
	 * A configuration file is not used the program has to perform many things:
	 * a) generate the optimal schema
	 * b) generate nodes positions
	 * c) generate listener position
	 */
	/*** a ***/
	/*compute the list of advertising cells to be used*/
	map<int, list<int> > advertisingCells = map<int, list<int> >();
	
	/*if I don't us a configuration file, let the schedule decide how to put the various cells*/
	if(configurationFile == false)
	{
		Schedule schedule = Schedule(N, advertiserChannels, numberAdvertisingCells);
		advertisingCells = schedule.computeSchedule();
	}
	
	/**
	 * If a configuration file is used, parse it and retrieve which is the proposed scheduling
	 * algorithm
	 */
	else
	{
		//cout<<"parser"<<endl;
		parsing parser = parsing("conf.txt");
		advertisingCells = parser.doParsing();
	}
	//cout << schedule << endl;
	
	cout << energyFactor << '\t' << numberAdvertisingCells <<endl;
	
	int tmpTimeslots[N - 1];
	int timeslots[N];
	for(int i = 0; i < N - 1; i++)
	{
		int in = i + 1;
		tmpTimeslots[i] = in;
	}
		
	int* tmpChannels = new int[advertiserChannels - 1];
	int* channels = new int[advertiserChannels];
	for(int i = 0; i < advertiserChannels; i++)
	{
		int in = i + 1;
		tmpChannels[i] = in;
	}
	
	for(int j = 0; j < topologiesToGenerate; j++)
	{
		Timeslot timeslot = Timeslot(random, transmissionRange, listenerChannels);
		timeslot.setEnergyFactor(energyFactor);
		listenerNode listener;
	
		//cout << "step " << j << endl;
		/*** b ***/
		//generate advertisers
		for(int i = 0; i < numberAdvertisers; i++)
		{
			//set available channels
			advNode node = advNode(advertiserChannels, transmissionRange);
			node.setRandom(random);
			
			/*if(fair == 1)
				node.setFairMethod(true);
			else
				node.setFairMethod(false);*/
			
			//insert link
			node.insertLinks(advertisingCells);
			
			//generate node position, check if the position is already occupied
			//position p = generatePosition(squareSide, random, tmpAdvNodes, transmissionRange);
			//node.setPosition(p);
			
			//cout << "Positions: " << p.x << "\t" << p.y << endl;
			
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
			node.setAvailableCells(numberAdvertisingCells);
			/*if(fair == 0)
			{
				//initialize the random advertising schemas
				node.initRandomAdvertising(RANDOMHORIZONTAL, NULL);
				node.initRandomAdvertising(RANDOMVERTICAL, NULL);
			}*/
			
			//insert nodes in lists
			tmpAdvNodes.push_back(node);
			timeslot.addNode(node);
		}
		//saveTopology(advNodes);
		//timeslot.setNodesCollisionProbability();
		/*** c ***/
		for(int lp = 0; lp < numberListenerPositions; lp++)
		{
			//cout<<"LisGen\n";
			//generate the listener
			bool acceptable = false;
			while(!acceptable) 
			{
				//set listener properties except its channel 
				//position p = generatePosition(squareSide, random, tmpAdvNodes, transmissionRange);
				//listener.xPos = p.x;
				//listener.yPos = p.y;
				acceptable = timeslot.addListener(listener);
			}
		}
			//cout<<"listener: "<< listener.xPos << "\t" << listener.yPos << endl;
			
		//timeslot.addListener(listener);
		for(int c = 0; c < iterations; c++)
		{
			//cout<<"iteration: "<<c<<endl;
			/*INITIALIZATION*/
			//do the shuffling
			//if(fair == 1)
			//{
				random_shuffle(tmpTimeslots, tmpTimeslots + N - 1);
				random_shuffle(tmpChannels, tmpChannels + advertiserChannels - 1);
				
				timeslots[0] = 0;
				for(int i = 1; i < N; i++)
					timeslots[i] = tmpTimeslots[i - 1];
				
				channels[0] = 0;
				for(int i = 0; i < advertiserChannels; i++)
					channels[i] = tmpChannels[i - 1];
				
				//at each iteration each node chooses the scheduling
				timeslot.changeScheduling(advertisingCells, timeslots, channels);
			//}
			
			//NOTE listener channels are picked here!!!
			timeslot.setListeningChannels();
			
			int tmp;
			//char t;
			//check if we need to add the probability
			if(ploss != 0)
				timeslot.setProbability(ploss);
			
			/*
			* via the timeslot manager gather:
			* in the fixed schema the number of framslot elapsed
			* in the others the number of timeslots elapsed
			*/
			
			//statistic for the optimum schema
			statStruct stat;
			/*if(ploss != 0)
			{
				stat.method = PLOSS_SCENARIO;
				stat.slotNumber = timeslot.timeslotManager(PLOSS_SCENARIO, &(stat.EBsent));
			}
			else
			{*/
				stat.method = OPTIMUM;
				tmp = timeslot.timeslotManager(OPTIMUM, &(stat.EBsent));
				stat.slotNumber = tmp;
				//cout<<stat.slotNumber<<'\t';
				
			//}
			statistics.statInsert(stat);
			
			//statistic for randomhorizontal
			stat.method = RANDOMHORIZONTAL;
			tmp = timeslot.timeslotManager(RANDOMHORIZONTAL, &(stat.EBsent));
			stat.slotNumber = tmp;
			statistics.statInsert(stat);
			//cout<<stat.EBsent<<'\t';
			
			//statistic for randomvertical
			stat.method = RANDOMVERTICAL;
			tmp = timeslot.timeslotManager(RANDOMVERTICAL, &(stat.EBsent));
			stat.slotNumber = tmp;
			statistics.statInsert(stat);
			//cout<<stat.slotNumber<<'\n';
			//char t;
			//cin>>t;
			//cout<<"method computed"<<endl;
			
		}
		
	
		//free resources before generating a new topology with the same density
		timeslot.erase();
		tmpAdvNodes.erase(tmpAdvNodes.begin(), tmpAdvNodes.end());
	}
	if(nameOfSchema.compare("") == 0 )
		nameOfSchema = "adv" + convertInt(numberAdvertisers) + "sq" + convertInt(squareSide);
	
	//compute and print statistics
	statistics.setTSIterations(iterations * numberListenerPositions * topologiesToGenerate);
	statistics.setEBIterations(iterations * topologiesToGenerate);
	statistics.print(nameOfSchema, RANDOMHORIZONTAL);
	statistics.print(nameOfSchema, RANDOMVERTICAL);
	statistics.print(nameOfSchema, OPTIMUM);
	delete tmpChannels;
	delete channels;
    return 0;
}
