#include "Random.h"


void Random::init()
{
	srand(time(NULL));
}

//get the next random number
int Random::getNumber(int max)
{
	int ran = random() % max;
	return ran;
}

/*
 * Get the next number generated between 0 and 1
 */
double Random::getNumber01()
{
	double ran = ( (float) random() / (RAND_MAX ) );
	//cout<<"generated number: "<<ran<<'\n';
	return ran;
	
}

//BEGIN TSCH_PART

/**
 * Initialization of the random class when using the tsch RGF
 * @param: nodeID this is the node_id (always DIFFERENT from 0) 
 */
void Random::tInit(int nodeID)
{
	next = nodeID;
	cout<<next<<endl;
}


int Random::tGetNumber(int max)
{
	int tmp;
	//unsigned int tmp2;
	next = next * 1103515245 + 12345;
	//this->next = tmp2;
	cout<<"tmp2: "<<next<<endl;
	tmp = ((next / 65536) % 32768);
	tmp = tmp % max;
	cout<<"tmp: "<<tmp<<endl;
	//setNext(tmp2);
	cout<<"R\tgen Number: "<<tmp<<'\t'<<max<<'\t'<<this->next<<'\t'<<endl;
	return tmp;
}

void Random::setNext(uint32_t n)
{
	this->next = n;
	cout<<this->next<<endl;
}

//END TSCH_PART

