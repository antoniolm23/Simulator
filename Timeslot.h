#include <cstdlib>
#include <iostream>
#include <list>
#include "Node.h"

class Timeslot{
	list<advNode> listNode;
	int listener;
public:
	Timeslot() {}
	void addNode(advNode a);
	bool compareChannel();
	void print();
	void addListener(int l);
	void changeChannel();
	void erase();
};