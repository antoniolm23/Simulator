#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <stdint.h>

using namespace std;

class Random
{
	uint32_t next;		//useful when using the tsch RGF {random generating function}
	//uint64_t tmp2;
	void setNext(uint32_t);
public:
	void init();
	void tInit(int);	//tsch initialization
	int getNumber(int m);
	double getNumber01();
	int tGetNumber(int m);	//tsch RGF
};