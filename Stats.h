#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>

using namespace std;

//an element of the statistics
struct statElem
{
	int timeslotNumber;	//denotes the timeslot nomber at which a "join" occurs
	int frequency;	//denotes how many times a node takes this amount of steps 
};

class Stat
{
	list<statElem> statsList;
	double computeStatistic();
public:
	Stat() {};
	void statInsert(int tsn);
	void print();
	void print(string t);
};