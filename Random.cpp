#include "Random.h"


Random::Random()
{
	srand(time(NULL));
}

//get the next random number
int Random::getNumber(int max)
{
	int ran = random() % max;
	return ran;
}



