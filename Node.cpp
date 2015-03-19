#include "Node.h"

advNode::advNode(int uc, int tc) 
{
	usedChannel = uc;
	availableChannels = tc;
	state = true;
}

/*
 * Function that returns the channel actually used by this node
 * @return: the channel used if node active, -1 otherwise
 */
int advNode::getUsedChannel() 
{
	if(state) 
	{
		return usedChannel;
	}
	else
		return -1;
}

//sets the used channel to uc
void advNode::setUsedChannel(int uc) 
{
	usedChannel = uc;
}

//sets the state of the node
void advNode::setState(bool s) 
{
	state = s;
}

//returns the state of the node
bool advNode::getState() 
{
	return state;
}

void advNode::print(ostream& os) 
{
	os<<"Node on channel "<<getUsedChannel()<<endl;
} 

//changes the beaconing channel of a node
void advNode::changeChannel() 
{
	usedChannel = (usedChannel + 1) % availableChannels;
}