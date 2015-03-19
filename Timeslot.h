#include <cstdlib>
#include <iostream>
#include <list>
#include "Node.h"

class Timeslot{
	list<advNode> listNode; //list of nodes
	list<advNode> activeNode;	//list of active nodes in that timeslot
	int listener;	//listener Channel
	int asn;	//absolute Sequence Number
	Random rand;
	
	void eraseActive();	//erase the list of active nodes
	void insertActive(advNode a); //inserts active nodes in the list
public:
	Timeslot(Random r) { asn = 0; rand = r;}
	void addNode(advNode a);
	bool compareChannel();
	void print();
	void addListener(int l);
	void changeChannel();
	void erase();
	void timeslotManager();
};