#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

class Random{
public:
	void init();
	int getNumber(int m);
	double getNumber01();
};