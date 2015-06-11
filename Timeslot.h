#include <cstdlib>
#include <iostream>
#include <list>
#include <cmath>
#include "Node.h"

class Timeslot{
	list<advNode> listNode; //list of nodes in the neighborhood
	list<advNode> neighbours;
	list<advNode> activeNode;	//list of active nodes in that timeslot
	listenerNode listener;	//listener Channel
	int asn;	//absolute Sequence Number
	
	int listenerChannels;
	
	Random rand;
	double ploss;	//probability of having a loss
	int method;		//method used to compute the timeslot
	double transmissionRange;
	
	bool solveUniformCollisions();
	bool solveDifferentCollisions();
	void eraseActive();	//erase the list of active nodes
	void insertActive(int); //inserts active nodes in the list
	int getRandomNumber(int, int);	//sets a new random number in listNode (only with TSCH random)
	bool allowableListener();
	void selectNeighbours();
public:
	Timeslot(Random, double, int); 
	void addNode(advNode a);
	bool compareChannel(int);
	void print();
	void setListenerChannel(int);
	void erase();
	int timeslotManager(int);
	void setProbability(double);
	bool setListener(listenerNode);
	void setNodesCollisionProbability();
};