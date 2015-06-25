#include <iostream>
#include <cstdlib>
#include <list>
#include <stdint.h>
#include <algorithm>
#include <vector>
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
	
	bool verticalCase;
	double verticalCollision;
	
	position pos;		//position of the node
	double radius;
	bool synchronized;
	
	Random random;	
	
	advLink randomHorizontal;
	advLink randomVertical;
	
	bool transmittingState;
	
public:
	advNode(int ac, double);
	int getUsedChannel(int, int);
	int getChannelOffset(int, int);
	void setState(bool s);
	void insertLink(int chOff, int ts);
	void insertLinks(const list< advLink >, Random );
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
	void initRandomAdvertising(int, Random);
	
	//handling different collisions
	void setTransmittingState(bool t) {transmittingState = t;}
	bool getTransmittingState() {return transmittingState;}
	
	//handling vertical collisions
	bool getVerticalState() {return verticalCase;}
	double getVerticalCollision() {return verticalCollision;}
	double generateNumber01(Random);
	
	void printLinks();
};