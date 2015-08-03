#include <iostream>
#include <cstdlib>
#include <list>
#include <stdint.h>
#include <algorithm>
#include <vector>
#include <map>
#include "define.h"
#include "Random.h"
using namespace std;

//this class defines an advertiser Node or coordinator
class advNode {
	
	map<int, list<int> > advertisingLinks;	//list of links used to do advertising
	int availableChannels;	//channels available
	bool state;			//state of the advertiser node (active or not)
	int absoluteChannel;
	int nodeId;
	
	int colliders;
	
	int type;
	
	bool verticalState;
	double verticalCollision;
	
	//position pos;		//position of the node
	double radius;
	bool synchronized;
	
	Random random;
	
	//list of timeslots at which an EB will be sent on channel 0
	list<int> randomHorizontal;
	
	//list of channels at which at timeslot 0 an EB will be sent
	list<int> randomVertical;
	
	bool transmittingState;
	
	bool fairMethod;
	int availableCells;
	
	void setVerticalState(bool) {verticalState = true;};
	
	int getFair(int* );
	
public:
	advNode(int ac, double);
	list<int> getUsedChannel(int, int);
	list<int> getChannelOffset(int, int);
	void setState(bool s);
	void insertLink(int chOff, int ts);
	void insertLinks(map<int, list<int> >);
	int generateNumber(int);
	int getAbsoluteChannel() { return absoluteChannel; }
	void setNodeID(int);
	//void setPosition(position);
	int getNodeID() { return nodeId; }
	void setType(int);
	void setSynchronization(bool );
	//double getPosX();
	//double getPosY();
	void setColliders(int c);
	int getColliders() {return colliders;}
	void initRandomAdvertising(int, int*);
	
	//handling different collisions
	void setTransmittingState(bool t) {transmittingState = t;}
	bool getTransmittingState() {return transmittingState;}
	
	//handling vertical collision
	bool getVerticalState() {return verticalState;}
	double getVerticalCollision() {return verticalCollision;}
	double generateNumber01();
	
	//set and get for fair fairMethod
	/**
	 * NOTE: in the fair method what happens is that 
	 * each node selects just one cell amond a subset of available 
	 * cells
	 */
	bool getFairMethod(){return fairMethod;};
	void setFairMethod(bool f) {fairMethod = f;};
	
	void setRandom(Random r) {random = r;};
	
	int generateNumberCollision(double, int);
	
	void setAvailableCells(int ac) {availableCells = ac;};
	
};