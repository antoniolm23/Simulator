#ifndef definitions
#define definitions
/*
 * LIST OF DEFINES
 */
#define N 23			//number of timeslots in a slotframe
#define CHSTART	11	//starting channel to determine the actual channel to use
//int he fixed schema nodes transmitt only in the first timeslot
#define FIXEDSCHEMA 1
//In the so-called staggered one to each node is assigned a transmission schema
#define STAGGERED 2
//here we have the staggered schema plus a ploss probability that is the probability of having a loss 
#define PLOSS_SCENARIO 4
#define CRT 3


#define COORDINATOR 1
#define ADVERTISER 2
#define LISTENER 0

#define TRANSMISSIONFLAG 0
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
#endif