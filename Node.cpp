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
}

/**
 * Function that returns the channel actually used by this node
 * according to the formula:
 * channel = ChStart + [(ASN + chOff) % Nc ]
 * @param: AbsoluteSequenceNumber
 * @return: the channel used if node active, -1 otherwise
 */
int advNode::getUsedChannel(int asn) 
{
	//determine timeslot number
	int timeslot = asn % N;
	
	int chOff;
	
	//getState returns the channelOffset if node has a link in that timeslot, -1 otherwise
	if((chOff = getState(timeslot)) != -1) {
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
 * Say if the node is sending beacons in this timeslot
 * @params: timeslot
 * @return: channel offset if node is active, -1 otherwise
 */
int advNode::getState(int timeslot) 
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
 * @param: the list of advertising links
 */
void advNode::insertLinks(list< advLink > a)
{
	advertisingLinks = a;
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

void advNode::setType(int t)
{
	type = t;
}




