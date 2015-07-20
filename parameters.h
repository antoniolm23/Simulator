/*
 * Purpose of this class is to read a file with various parameter configuration, one for
 * each node, and then return to the main program, the configuration of each node, so that it 
 * avoids the manual insertion of each parameter each time
 * 
 */
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <sstream>
#include <map>
using namespace std;

/*
 * a parameter is a struct of the form:
 * NodeID, ChannelOffset, Timeslot
 */
struct parameter{
	int nodeID;
	int chOff;
	int timeslot;
};

class parsing{
	string filename;
	//list<parameter> parameterList;
	map<int, list<int> > schedule;
	int parameterListSize;
public:
	parsing(string fname) { filename = fname; schedule = map<int, list<int> >(); };
	map<int, list<int> > doParsing();
	parameter getParameter() ;
	int getParameterSize() ;
};