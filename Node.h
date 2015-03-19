#include <iostream>
#include <cstdlib>
#include <list>
#include "define.h"
#include "Random.h"
using namespace std;

/*
 * This struct represents the link used to do advertising
 * a link is a couple (timeslot, channelOffset) assigned.
 * To each node is assigned from 0 to N (where N is the 
 * dimension of the slotframe) links
 */
struct advLink{
	int channelOffset;	//channel offsets used to advertise
	int timeslot;	//timeslot in the slotframe used to do advertising
};

//this class defines an advertiser Node
class advNode {
	list<advLink> advertisingLinks;	//list of links used to do advertising
	int availableChannels;	//channels available
	bool state;			//state of the advertiser node (active or not)
public:
	advNode(int ac);
	int getUsedChannel();
	int getState();
	void setState(bool s);
	void insertLink(int chOff, int ts);
	int generateNumber(int, Random);	
};