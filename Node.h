#include <iostream>
#include <cstdlib>
#include <list>
#include <stdint.h>
#include "define.h"
#include "Random.h"
using namespace std;



//this class defines an advertiser Node or coordinator
class advNode {
	
	list<advLink> advertisingLinks;	//list of links used to do advertising
	int availableChannels;	//channels available
	bool state;			//state of the advertiser node (active or not)
	int absoluteChannel;
	int nodeId;
	
	int colliders;
	
	int type;
	
	position pos;		//position of the node
	double radius;
	bool synchronized;
	
	Random tRandom;	
	
public:
	advNode(int ac, double);
	int getUsedChannel(int);
	int getState(int);
	void setState(bool s);
	void insertLink(int chOff, int ts);
	void insertLinks(list<advLink>);
	int generateNumber(int, Random);
	int getAbsoluteChannel() { return absoluteChannel; }
	void setNodeID(int);
	void setPosition(position);
	int getNodeID() { return nodeId; }
	void setType(int);
	void setSynchronization(bool );
	int getPosX();
	int getPosY();
	void setColliders(int c) {colliders = c;}
	int getColliders() {return colliders;}
};