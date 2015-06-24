#ifndef definitions
#define definitions
/*
 * LIST OF DEFINES
 */
#define N 101			//number of timeslots in a slotframe
#define CHSTART	11	//starting channel to determine the actual channel to use
//int he fixed schema nodes transmitt only in the first timeslot
#define FIXEDSCHEMA 1
//In the so-called staggered one to each node is assigned a transmission schema
#define OPTIMUM 2
//here we have the staggered schema plus a ploss probability that is the probability of having a loss 
#define PLOSS_SCENARIO 4
#define CRT 3
#define RANDOMVERTICAL 5
#define RANDOMHORIZONTAL 6

#define COORDINATOR 1
#define ADVERTISER 2
#define LISTENER 0

#define TRANSMISSIONFLAG 0

#define CONFIDENCE95 	1.96

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
/**
 * This struct represents the link used to do advertising
 * a link is a couple (timeslot, channelOffset) assigned.
 * To each node is assigned from 0 to N (where N is the 
 * dimension of the slotframe) links
 */
struct advLink{
	int channelOffset;	//channel offsets used to advertise
	int timeslot;	//timeslot in the slotframe used to do advertising
};

struct position
{
	int x;
	int y;
};

struct listenerNode
{
	int xPos;
	int yPos;
	int channelUsed;
};

//provides a timeslot counter and a method indication
struct statStruct
{
	int EBsent;
	int slotNumber;
	int method;
};
#endif