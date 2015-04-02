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



