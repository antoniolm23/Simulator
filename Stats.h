#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include "define.h"

using namespace std;

//an element of the statistics
struct statElem
{
	int timeslotNumber;	//denotes the timeslot nomber at which a "join" occurs
	int frequency;	//denotes how many times a node takes this amount of steps
	int method;
};

class Stat
{
	list<statElem> statsList;
	double computeStatistic(int);
	
	int iterations;
public:
	Stat() {};
	void statInsert(statStruct);
	void setIterations(int);
	void print();
	void print(string t, int);
};