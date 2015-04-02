/*
 * LIST OF DEFINES
 */
#define N 3			//number of timeslots in a slotframe
#define CHSTART	11	//starting channel to determine the actual channel to use
//int he fixed schema nodes transmitt only in the first timeslot
#define FIXEDSCHEMA 1
//In the so-called staggered one to each node is assigned a transmission schema
#define STAGGERED 2
//here we have the staggered schema plus a ploss probability that is the probability of having a loss 
#define PLOSS_SCENARIO 4
#define CRT 3