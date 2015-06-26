#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include <cmath>
#include "define.h"

using namespace std;

//an element of the statistics
/*struct statElem
{
	int timeslotNumber;	//denotes the timeslot nomber at which a "join" occurs
	int frequency;	
	int method;
};*/

class Stat
{
	list<statStruct> statsList;
	double computeMeanSlotNumber(int);
	double computeMeanEBsent(int);
	double computeConfidenceIntervalSlotNumber(int, double);
	double computeConfidenceIntervalEBsent(int, double);
	
	int TSiterations;
	int EBiterations;
public:
	Stat() {};
	void statInsert(statStruct);
	void setTSIterations(int i) {TSiterations = i;}
	void setEBIterations(int i) {EBiterations = i;}
	void print();
	void print(string t, int);
};