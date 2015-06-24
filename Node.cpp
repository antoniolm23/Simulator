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
	verticalCase = false;
	transmittingState = false;
	idNeighbours = list<int>();
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
		//search if there is a link with that timeslot
		for(list<advLink>::iterator it = advertisingLinks.begin(); it != advertisingLinks.end(); ++it) 
		{
			if(it -> timeslot == timeslot)
			{
				//cout<<"active "<<nodeId<<endl;
				return it->channelOffset;
			}
			/*else
				cout<<"inactive "<<nodeId<<endl;*/
		}
	}
	return -1;
}

/**
 * Insert a new Advertising link for this node
 * @params: channelOffset, timeslot
 */
void advNode::insertLink(int chOff, int ts)
{
	//create the structure
	advLink aL;
	aL.channelOffset = chOff;
	aL.timeslot = ts;
	
	//cout<<nodeId<<" insert: "<<aL.channelOffset<<aL.timeslot<<endl;
	
	//insert in the list
	advertisingLinks.push_back(aL);
}


/**
 * If there is a collision on a link, the node is asked to transmit with 
 * a certain probability, hence, given this number, the timeslot manager
 * knows to which node allocate that link
 * @param: number of colliding nodes
 * @return: generated number
 * TODO: add a way to choose between c++ random number generator and tsch one
 */
int advNode::generateNumber(int max, Random r)
{
	return r.getNumber(max); 
	/*
	* For this test purposes we will use the tsch random generator 
	* function
	*/
	/*int tmp;
	//cout<<"nextR: "<<nextRandom<<endl;
	this->nextRandom = this->nextRandom * 1103515245 + 12345;
	//this->next = tmp2;
	//cout<<"tmp2: "<<this->nextRandom<<endl;
	tmp = ((this->nextRandom / 65536) % 32768);
	tmp = tmp % max;
	//cout<<"tmp: "<<tmp<<endl;
	//setNext(tmp2);
	//cout<<"R\tgen Number: "<<tmp<<'\t'<<max<<'\t'<<this->nextRandom<<'\t'<<endl;
	//this->nextRandom = this->nextRandom;
	return tmp;
	//}*/
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
 * NOTE: hanldes the cases in which the optimal case uses the same timeslot with 
 * different channelOffsets
 * @param: the list of advertising links, random class
 */
void advNode::insertLinks(const list< advLink > a, Random random)
{
	
	int possibleChannelOffsets[16];
	int timeslot;
	int occurrences = 0;
	
	list<advLink> tmp = list<advLink>(a);
	list<int> usedTimeslots = list<int>();
	/**
	 * Check if the optimal schedule provides many transmission at the same
	 * timeslot and at different channelOffset, in this case pick just one of the
	 * possible channelOffset
	 */
	for(list<advLink>::const_iterator it = a.begin(); it != a.end(); ++it) 
	{
		timeslot = it->timeslot;
		/**
		 * if a new timeslot is detected, and only in that case, 
		 * check if multiple channelOffsets are used
		 */ 
		if(find(usedTimeslots.begin(), usedTimeslots.end(), timeslot) == usedTimeslots.end())
		{
			usedTimeslots.push_front(timeslot);
			for(list<advLink>::iterator jt = tmp.begin(); jt != tmp.end(); ++jt)
			{
				list<advLink>::iterator eraseIt = jt;
				/**
				* all equal timeslot will form a list from which each node picks its 
				*/
				if(timeslot == jt -> timeslot)
				{
					possibleChannelOffsets[occurrences] = jt->channelOffset;
					occurrences++;
				}
			}
			
			//pick the chosen channel offset and add it to the list
			if(occurrences > 1)
			{
				int ts = random.getNumber(occurrences);
				int chosenChannelOffset = possibleChannelOffsets[ts];
				insertLink(chosenChannelOffset, timeslot);
				verticalCase = true;
				verticalCollision = occurrences;
				//cout << verticalCollision << endl;
			}
			
			//otherwise insert link in the list
			else
			{
				insertLink(it -> channelOffset, it -> timeslot);
			}
			occurrences = 0;
		}
	}
	
	//delete everything
	usedTimeslots.clear();
	tmp.clear();
	
}

//return single coordinates of the node position
int advNode::getPosX()
{
	return pos.x;
}
int advNode::getPosY()
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
 * TODO PART
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
void advNode::initRandomAdvertising(int method, Random r)
{
	/*Initialization according to RandomHorizontal schema*/
	if(method == RANDOMHORIZONTAL)
	{
		if(type == COORDINATOR)
		{
			randomHorizontal.channelOffset = randomHorizontal.timeslot = 0;
		}
		else
		{
			//select a random timeslot
			int timeslot = r.getNumber(N);
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
			int channelOffset = r.getNumber(availableChannels);
			randomVertical.channelOffset = channelOffset;
			randomVertical.timeslot = 0;
		}
		
		//cout << randomVertical.timeslot << '\t' << randomVertical.channelOffset << endl;
	}
}


/**
 * print the list of links used by the node
 */
void advNode::printLinks()
{
	for(list<advLink>::iterator it = advertisingLinks.begin(); it != advertisingLinks.end(); ++it)
	{
		cout << it ->timeslot << ":\t" << it->channelOffset << endl;
	}
}

/**
 * generate a random number between 0 and 1
 */
double advNode::generateNumber01(Random random)
{
	return random.getNumber01();
}

/**
 * Set the list of the id of neighbours, useful in case of 
 * collision to detect if the colliding nodes are in mutual communication range
 * @param: the various IDs 
 */  
void advNode::setIdNeighbours(list<int> ids)
{
	
	for(list<int>::iterator it = ids.begin(); it != ids.end(); ++it)
	{
		idNeighbours.push_back(*it);
	}
}

/**
 * Search in the list of idNeighbour for a given id
 * @return: true if the id is present, false otherwise
 */
bool advNode::findIdNeighbour(int id)
{
	if(idNeighbours.empty())
		return false;
	
	for(list<int>::iterator it = idNeighbours.begin(); it != idNeighbours.end(); ++it)
	{
		if(*it == id)
			return true;
	}
	return false;
}

void advNode::eraseIdNeighbours()
{
	idNeighbours.clear();
}
