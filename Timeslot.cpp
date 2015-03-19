#include "Timeslot.h"

void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
}

//searches for a match throughout the list of advertisers
bool Timeslot::compareChannel()
{
	//scan all the list looking for a match
	for( list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it  )
	{
		if(it->getUsedChannel() == listener)
			return true;
	}
	return false;
}

//adds the listener channel
void Timeslot::addListener(int l)
{
	listener = l;
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

//provides the multichannel ability
void Timeslot::changeChannel()
{
	for( list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it )
		it->changeChannel();
}

//delete all the list
void Timeslot::erase()
{
	listNode.erase(listNode.begin(), listNode.end());
}





