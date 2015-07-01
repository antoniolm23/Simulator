#include <cstdlib>
#include <iostream>
#include <list>
#include <cmath>
#include "Node.h"
#include <vector>
#include <algorithm>

class Timeslot{
	list<advNode> listNode; //list of nodes in the neighborhood
	list<advNode> activeNode;	//list of active nodes in that timeslot
	list<listenerNode> listenersList;	//listener Channel
	int asn;	//absolute Sequence Number
	
	bool printProb;
	
	int listenerChannels;
	
	Random rand;
	double ploss;	//probability of having a loss
	int method;		//method used to compute the timeslot
	double transmissionRange;
	
	double energyFactor;	//denominator only
	
	bool solveUniformCollisions();
	bool solveDifferentCollisions(int*);
	void eraseActive();	//erase the list of active nodes
	void insertActive(int); //inserts active nodes in the list
	int getRandomNumber(int, int);	//sets a new random number in listNode (only with TSCH random)
	bool allowableListener(listenerNode);
public:
	Timeslot(Random, double, int); 
	void addNode(advNode a);
	bool compareChannel(int, listenerNode);
	void print();
	void erase();
	int timeslotManager(int, double*);
	void setProbability(double);
	bool addListener(listenerNode);
	void setNodesCollisionProbability();
	void setEnergyFactor(int e) {energyFactor = e;}
};