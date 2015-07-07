#include "Node.h"

/**
 * Constructor of an advertiser node
 * @param: number of available channels, transmitting range
 */
advNode::advNode(int tc, double r) 
{
	availableChannels = tc;
	state = true;
	radius = r;
	verticalState = false;
	transmittingState = false;
}

/**
 * Function that returns the channel actually used by this node
 * according to the formula:
 * channel = ChStart + [(ASN + chOff) % Nc ]
 * @param: AbsoluteSequenceNumber, method used
 * @return: the channel used if node active, -1 otherwise
 */
int advNode::getUsedChannel(int asn, int method) 
{
	//determine timeslot number
	int timeslot = asn % N;
	
	int chOff;
	
	//getState returns the channelOffset if node has a link in that timeslot, -1 otherwise
	if((chOff = getChannelOffset(timeslot, method)) != -1) {
		int usedChannel = CHSTART + ((asn + chOff) % availableChannels);
		absoluteChannel = usedChannel;
		return usedChannel;
	}
	
	else
		return -1;
}


/**
 * sets the state of the node
 * @param: state of the node, true stands for active, 
 * false stands for inactive
 */
void advNode::setState(bool s) 
{
	state = s;
}

/**
 * Get the channel offset used by the node 
 * according to the method used to perform advertising,
 * -1 if the node is not active
 * @params: timeslot, the method used to transmitt EB
 * @return: channel offset if node is active, -1 otherwise
 */
int advNode::getChannelOffset(int timeslot, int method) 
{
	if(method == RANDOMVERTICAL)
	{
		if(randomVertical.timeslot == timeslot)
			return randomVertical.channelOffset;
	}
	
	if(method == RANDOMHORIZONTAL)
	{
		if(randomHorizontal.timeslot == timeslot)
			return randomHorizontal.channelOffset;
	}
	
	if(method == PLOSS_SCENARIO || method == OPTIMUM)
	{
		map<int, list<int> >::iterator it;
		it = advertisingLinks.find(timeslot);
		if(it != advertisingLinks.end())
		{
			if( getVerticalState() )
			{
				//cout<<"getChannelOffset\n";
				int pos = random.getNumber( it -> second.size() );
				list<int>::iterator listIt = it -> second.begin();
				for(int i = 0; i < pos; i++)
				{
					++listIt;
				}
				//cout <<nodeId<< ": ChannelOffset: " << *listIt << endl;
				return *listIt;
			}
			else
				return *it->second.begin();
		}
	}
	return -1;
}

/**
 * If there is a collision on a link, the node is asked to transmit with 
 * a certain probability, hence, given this number, the timeslot manager
 * knows to which node allocate that link
 * @param: number of colliding nodes
 * @return: generated number
 * TODO: add a way to choose between c++ random number generator and tsch one
 */
int advNode::generateNumber(int max)
{
	return random.getNumber(max); 
}

void advNode::setNodeID(int id)
{
	nodeId = id;
	
	/*
	 * initializes the tsch random function with 
	 * the nodeID, this initialization is requested
	 * for test puposes in fact the C++ rand and the
	 * one used in tsch have different collision probabilities
	 * i.e. in the tsch one there is an higher probability 
	 * that the nodes generate the same number
	 */
	//tRandom.tInit(nodeId);
	//nextRandom = id;
}

/**
 * Function to insert the list of advertising links for each advertising node
 * 
 * NOTE: handles the cases in which the optimal case uses the same timeslot with 
 * different channelOffsets
 * @param: the list of advertising links, random class
 */
void advNode::insertLinks(map< int, list<int> > scheduling)
{
	/**
	 * if we don't use a fair method, the node tries to transmitt in each of the 
	 * available cells
	 */
	if(fairMethod == false)
	{
		advertisingLinks = map<int, list<int> >(scheduling);
		
		//check if this is a vertical scheduling
		for(map<int, list<int> >::iterator it = scheduling.begin(); it != scheduling.end(); ++it )
		{
			if(it->second.size() > 1)
			{
				verticalCollision = it->second.size();
				setVerticalState(true);
				return;
			}
		}
	}
	
	/**
	 * In this case the node has to pick just one cell and it
	 * can transmitt only in this cell 
	 */
	if(fairMethod == true)
	{
		advertisingLinks.clear();
		
		if(type == COORDINATOR)
		{
			advertisingLinks[0].push_back(0);
			return;
		}
		advertisingLinks.clear();
		int max = 0;
		for(map<int, list<int> >::iterator it = scheduling.begin(); it != scheduling.end(); ++it )
		{
			max += it -> second.size();
		}
		availableCells = max;
		int cellIndex = generateNumber(max);
#ifdef DEBUG
		cout<<"Availablecells: "<<availableCells<<'\t'<<cellIndex<<endl;
#endif
		//go to the right index
		int index = 0;
		for(map<int, list<int> >::iterator it = scheduling.begin(); it != scheduling.end(); ++it )
		{
			for(list<int>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			{
				if(index < cellIndex)
					index++;
				else
				{
					advertisingLinks[it->first].push_back(*jt);
#ifdef DEBUG
					cout<<"UsedCell: "<<it->first<<'\t'<<*jt<<endl;
#endif
					return;
				}
			}
		}
	}
	
}

//return single coordinates of the node position
double advNode::getPosX()
{
	return pos.x;
}
double advNode::getPosY()
{
	return pos.y;
}

/**
 * Set the node position of the node
 * @param: postion of the node
 */
void advNode::setPosition(position p)
{
	pos.x = p.x;
	pos.y = p.y;
	
}

/**
 * If the advertiser is not synchronized has to wait for the first beacon to arrive
 */
void advNode::setSynchronization(bool advertiserSynchronized)
{
	synchronized = advertiserSynchronized;
}
//set the type of advertiser node, it can be coordinator or advertiser
void advNode::setType(int t)
{
	type = t;
}

/**
 * initialize the node advertising schema according to random horizontal or random vertical
 */
void advNode::initRandomAdvertising(int method, int* vect)
{
	/*Initialization according to RandomHorizontal schema*/
	if(fairMethod == false || 
		(availableCells >= availableChannels && method == RANDOMVERTICAL) 
		|| availableCells >= N)
	{
		if(method == RANDOMHORIZONTAL)
		{
			if(type == COORDINATOR)
			{
				randomHorizontal.channelOffset = randomHorizontal.timeslot = 0;
			}
			else
			{
				//select a random timeslot
				int timeslot = random.getNumber(N);
				randomHorizontal.timeslot = timeslot;
				randomHorizontal.channelOffset = 0;
			}
			
		}
		
		/*initialization according to RandomVertical schema*/
		if(method == RANDOMVERTICAL) 
		{
			if(type == COORDINATOR)
			{
				randomVertical.channelOffset = randomVertical.timeslot = 0;
			}
			else
			{
				int channelOffset = random.getNumber(availableChannels);
				randomVertical.channelOffset = channelOffset;
				randomVertical.timeslot = 0;
			}
			
			//cout << randomVertical.timeslot << '\t' << randomVertical.channelOffset << endl;
		}
	}
	
	if(fairMethod == true)
	{
		if(method == RANDOMHORIZONTAL)
		{
			if(type == COORDINATOR)
			{
				randomHorizontal.channelOffset = randomHorizontal.timeslot = 0;
			}
			else
			{
				
				//select a random timeslot among a subset of available cells
				int timeslot = getFair(vect);
				randomHorizontal.timeslot = timeslot;
				randomHorizontal.channelOffset = 0;
			}
#ifdef DEBUG
			cout << "RH: " << randomHorizontal.timeslot << '\t' << randomHorizontal.channelOffset << endl;
#endif
		}
		
		/*initialization according to RandomVertical schema*/
		if(method == RANDOMVERTICAL) 
		{
			if(type == COORDINATOR)
			{
				randomVertical.channelOffset = randomVertical.timeslot = 0;
			}
			else
			{
				int channelOffset = getFair(vect);
				randomVertical.channelOffset = channelOffset;
				randomVertical.timeslot = 0;
			}
#ifdef DEBUG
			cout << "RV: " << randomVertical.timeslot << '\t' << randomVertical.channelOffset << endl;
#endif
		}
	}
}



/**
 * generate a random number between 0 and 1
 */
double advNode::generateNumber01()
{
	return random.getNumber01();
}

/**
 * Set the number of colliders nodes
 */
void advNode::setColliders(int c)
{
	if(c > 0)
		colliders = c;
	else
		colliders = 1;
#ifdef DEBUG
	//cout<<"Colliders:\t"<<colliders<<'\t'<<c<<endl;
#endif
}

/**
 * In case of fair method I have to select a cell among a subset of the available ones
 * mixed by a shuffle method
 * @param: array of mixed cell
 * @return: the chosen one
 */
int advNode::getFair(int* vect)
{
	int index = random.getNumber(availableCells);
	return vect[index];
}

/**
 * generate a random number to avoid collision, the generated number depends on 
 * a probability that is stated according to the method used (OPTIMUM, RV, RH..) 
 * and basing on the fact that we're fair or not 
 */
int advNode::generateNumberCollision(int prob, int method)
{
	/*if(method == OPTIMUM && fairMethod == false)
		prob = prob * getColliders(); */
	return random.getNumber(prob);
}

