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


bool search(position p, list<advNode> listAdv, int transmissionRange)
{
	bool inTxRange = false;
	bool correct = false;
	for(list<advNode>::iterator it = listAdv.begin(); it != listAdv.end(); ++it)
	{
		int result;
		result = checkNeighbours(p.x, p.y, it->getPosX(), it -> getPosY(), transmissionRange);
		//cout<<result<<'\n';
		
		if(result == INTXRANGE)
			inTxRange = true;
		if(result == AVAILABLEPOS)
			correct = true;
		if(result == OCCUPIED)
			return false;
		
	}
	if(inTxRange == true)
	{
		//cout<<"Accepted\n";
		return true;
	}
	else
		return false;
}

//utility function to generate positions of advertiser nodes
position generatePosition(int squareSide, Random random, 
						  list<advNode> listAdv, int transmissionRange)
{
	//cout<<"Generate position"<<endl;
	bool acceptable = false;
	position p;
	
	//select an advertiser neighbour
	if(listAdv.size() != 0)
	{
		int pos = random.getNumber(listAdv.size());
		list<advNode>::iterator it = listAdv.begin();
		for( int i = 0; i < pos; i++)
			++it;
		
		int result = 0;
		
		while(!acceptable)
		{
			double x = it -> getPosX();
			double y = it -> getPosY();
			
			double a = random.getNumber01();
			double b = random.getNumber01();
			
			
			if (b < a)
			{
				double tmp = a;
				a = b;
				b = tmp;
			}
			
			//generate a point in the circle
			p.x = b * transmissionRange * cos(2 * M_PI * a /b) + x;
			p.y = b * transmissionRange * sin(2 * M_PI * a / b) + y;
			
			if(p.x > 0 && p.y > 0)
			{
				result = checkNeighbours(p.x, p.y, x, y, transmissionRange);
				
				if(result == INTXRANGE)
				{
					acceptable = true;
				}
				else
				{
					cout << "uncorrect\n";
				}
			}
		}
	}
	
	if(listAdv.size() == 0)
	{
		p.x = random.getNumber01() * squareSide;
		p.y = random.getNumber01() * squareSide;
	}
	//cout<<"Accepted1\t"<<p.x<<'\t'<<p.y<<'\n';
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
 * -f configurationFile in order to enable the possibility of using a configuration file too
 * -C advertising cells to be used
 * -P possible listener positions
 * -E energy factor
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
	int numberAdvertisers = 10;
	bool advertiserSynchronized = true;
	int numberAdvertisingCells = 0;
	string nameOfSchema = "";
	double ploss = 0.0;
	int numberListenerPositions = 10;
	int energyFactor = 2;
	int fair = 0;
	
	//parsing passed parameters
    while((c = getopt(argc, argv, "i:a:c:l:s:p:t:S:T:y:C:P:E:f:")) != -1) 
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
				break;
			case 'P':
				numberListenerPositions = atoi(optarg);
				break;
			case 'E':
				energyFactor = atoi(optarg);
				break;
			case 'f':
				fair = atoi(optarg);
				break;
			case '?':
				cout<<optarg<<endl;
				break;
		}
	}
	
	cout << energyFactor <<endl;
	
	/**
	 * If the nubmer of advertising cells is not defined, select a number of advertising cells 
	 * according to nodes density
	 */
	if(numberAdvertisingCells == 0)
	{
		double transmissionArea = pow(transmissionRange, 2) * M_PI;
		double squareArea = pow(squareSide, 2);
		double density = numberAdvertisers / squareArea;
		cout << squareArea << '\t' << numberAdvertisers << '\t' << density <<endl;
		numberAdvertisingCells = (int) floor(transmissionArea * density);
		
		//error checking
		/**
		 * if(numberAdvertisingCells < 2)
		 * numberAdvertisingCells = 2;
		 * numberAdvertisingCells = numberAdvertisingCells + 1;
		 */
		//FIXME
		if(transmissionRange == 10)
		{
			if(numberAdvertisers == 5)
				numberAdvertisingCells = 4;
			if(numberAdvertisers == 10)
				numberAdvertisingCells = 5;
			if(numberAdvertisers == 20)
				numberAdvertisingCells = 7;
			if(numberAdvertisers == 30)
				numberAdvertisingCells = 8;
			if(numberAdvertisers == 40)
				numberAdvertisingCells = 9;
			if(numberAdvertisers == 50)
				numberAdvertisingCells = 10;
			if(numberAdvertisers == 75)
				numberAdvertisingCells = 12;
			if(numberAdvertisers == 100)
				numberAdvertisingCells = 14;
			if(numberAdvertisers == 125)
				numberAdvertisingCells = 16;
			if(numberAdvertisers == 150)
				numberAdvertisingCells = 19;
			if(numberAdvertisers == 200)
				numberAdvertisingCells = 24;
			if(numberAdvertisers == 250)
				numberAdvertisingCells = 29;
		}
		if(numberAdvertisingCells > 101)
			numberAdvertisingCells = 101;
		
		cout << "density: " << density << "\ttransmissionArea: " << transmissionArea 
			<< "\tnumberAdvertisingCells: " << numberAdvertisingCells << endl;
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
	Schedule schedule = Schedule(N, advertiserChannels, numberAdvertisingCells);
	advertisingCells = schedule.computeSchedule();
	//cout << schedule << endl;
	
	int tmpTimeslots[N - 1];
	int timeslots[N];
		for(int i = 0; i < N - 1; i++)
		{
			int in = i+1;
			tmpTimeslots[i] = in;
		}
		
	int* tmpChannels = new int[advertiserChannels - 1];
	int* channels = new int[advertiserChannels];
	for(int i = 0; i < advertiserChannels; i++)
		tmpChannels[i] = i;
	
	for(int j = 0; j < topologiesToGenerate; j++)
	{
		Timeslot timeslot = Timeslot(random, transmissionRange, listenerChannels);
		timeslot.setEnergyFactor(energyFactor);
		listenerNode listener;
	
		cout << "step " << j << endl;
		/*** b ***/
		//generate advertisers
		for(int i = 0; i < numberAdvertisers; i++)
		{
			//set available channels
			advNode node = advNode(advertiserChannels, transmissionRange);
			node.setRandom(random);
			
			if(fair == 1)
				node.setFairMethod(true);
			else
				node.setFairMethod(false);
			
			//insert link
			node.insertLinks(advertisingCells);
			
			//generate node position, check if the position is already occupied
			position p = generatePosition(squareSide, random, tmpAdvNodes, transmissionRange);
			node.setPosition(p);
			
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
			
			if(fair == 0)
			{
				//initialize the random advertising schemas
				node.initRandomAdvertising(RANDOMHORIZONTAL, NULL);
				node.initRandomAdvertising(RANDOMVERTICAL, NULL);
			}
			
			//insert nodes in lists
			tmpAdvNodes.push_back(node);
			timeslot.addNode(node);
		}
		//saveTopology(advNodes);
		timeslot.setNodesCollisionProbability();
		/*** c ***/
		for(int lp = 0; lp < numberListenerPositions; lp++)
		{
			//cout<<"LisGen\n";
			//generate the listener
			bool acceptable = false;
			while(!acceptable) 
			{
				//set listener properties except its channel 
				position p = generatePosition(squareSide, random, tmpAdvNodes, transmissionRange);
				listener.xPos = p.x;
				listener.yPos = p.y;
				acceptable = timeslot.addListener(listener);
			}
		}
			//cout<<"listener: "<< listener.xPos << "\t" << listener.yPos << endl;
			
		//timeslot.addListener(listener);
		for(int c = 0; c < iterations; c++)
		{
			cout<<"iteration: "<<c<<endl;
			/*INITIALIZATION*/
			//do the shuffling
			if(fair == 1 && (c % 1000 == 0))
			{
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
			}
			//channels are picked here!!!
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
			if(ploss != 0)
			{
				stat.method = PLOSS_SCENARIO;
				stat.slotNumber = timeslot.timeslotManager(PLOSS_SCENARIO, &(stat.EBsent));
			}
			else
			{
				stat.method = OPTIMUM;
				tmp = timeslot.timeslotManager(OPTIMUM, &(stat.EBsent));
				stat.slotNumber = tmp;
				//cout<<stat.slotNumber<<'\t';
				
			}
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
