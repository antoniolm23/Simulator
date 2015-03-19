#include "Timeslot.h"

/*
 * Functions to insert a node in one of the lists
 */
void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
}
void Timeslot::insertActive(advNode a)
{
	activeNode.push_back(a);
}

/*
 * adds the listener channel
 * @params: listener channel without ChannelStart
 */
void Timeslot::addListener(int l)
{
	listener = l + CHSTART;
}

//searches for a match throughout the list of advertisers
bool Timeslot::compareChannel()
{
	//scan all the list looking for a match
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		if(it->getUsedChannel() == listener)
			return true;
	}
	return false;
}

void Timeslot::print()
{
	cout<<listener<<endl;
	for( list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it  )
	{
		cout<<'\t'<<it->getUsedChannel();
	}
	cout<<endl<<"******************************************************"<<endl;
}

//delete all the list
void Timeslot::erase()
{
	listNode.erase(listNode.begin(), listNode.end());
}

/*
 * Function that operates the most operations on a timeslot:
 * 1) arranges links in the timeslot
 * 2) checks wether or not channels are equal 
 * 3) operates on a timeslot Base
 * 4) if method is fixed returns the number of frameslot elapsed
 * 
 * @params: method used
 * @return: framselot elapsed
 */



