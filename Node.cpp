#include "Node.h"

advNode::advNode(int tc) 
{
	availableChannels = tc;
	state = true;
}

/*
 * Function that returns the channel actually used by this node
 * according to the formula:
 * channel = ChStart + [(ASN + chOff) % Nc ]
 * @params: AbsoluteSequenceNumber
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


//sets the state of the node
void advNode::setState(bool s) 
{
	state = s;
}

/*
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
			return it->channelOffset;
	}
	
	return -1;
}

/*
 * Insert a new Advertising link for this node
 * @params: channelOffset, timeslot
 */
void advNode::insertLink(int chOff, int ts)
{
	//create the structure
	advLink aL;
	aL.channelOffset = chOff;
	aL.timeslot = ts;
	
	//insert in the list
	advertisingLinks.push_back(aL);
}


/*
 * If there is a collision on a link, the node is asked to transmit with 
 * a certain probability, hence, given this number, the timeslot manager
 * knows to which node allocate that link
 * @params: number of colliding nodes
 * @return: generated number
 */
int advNode::generateNumber(int max, Random r)
{
	return r.getNumber(max);
}