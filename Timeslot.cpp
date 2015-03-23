#include "Timeslot.h"

/*
 * Functions to insert a node in one of the lists
 */
void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
}
void Timeslot::insertActive(int asn)
{
	int channelUsed;
	
	for( list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it  )
	{
		//if node active then add a record in the list
		if((channelUsed = it -> getUsedChannel(asn)) != -1) 
		{
			activeNode.push_back(*it);
		}
	}
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
		if(it->getAbsoluteChannel() == listener)
			return true;
	}
	return false;
}

void Timeslot::print()
{
	cout<<listener<<endl;
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		cout<<'\t'<<it->getNodeID()<<' '<<it->getAbsoluteChannel();
	}
	cout<<endl<<"**************************************"<<endl;
}

//delete all the list
void Timeslot::erase()
{
	listNode.erase(listNode.begin(), listNode.end());
	eraseActive();
}
void Timeslot::eraseActive()
{
	activeNode.erase(activeNode.begin(), activeNode.end());
}

//find if the max value available (size-1) is present in the list 
int findMax(int* v, int size) 
{
	int max = size - 1;
	int totMax = 0;
	for(int i = 0; i < size; i++) {
		if(max == v[i])
			totMax++;
	}
	if(totMax == 1)
		return true;
	else
		return false;
}

//finds int t in list l
bool find(list<int> l, int t) {
	if(l.empty())
		return false;
	for( list<int>::iterator it = l.begin(); it != l.end(); ++it  )
	{
		//cout<<"list scanning"<<endl;
		if(*it == t)
			return true;
	}
	return false;
}

/*
 * Try to solve a collision in the same timeslot if there are more thn one node that want to transmitt
 * @return: true -> collision solved, false collision or colliding nodes are off
 */
bool Timeslot::solveCollisions()
{
	//cout<<"to solve collisons"<<endl;
	
	if(activeNode.size() < 2)
		return true;
	
	int usedBy = 0;
	list<int> tmpChannel;
	list<advNode> colliding;
	
	bool collisionSolved;
	
	//check if the same channel is used by more than one node
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		//if the channel is not already in the list then push it in the list
		if(!find(tmpChannel, it->getAbsoluteChannel())) {
			tmpChannel.push_back(it -> getAbsoluteChannel());
			for( list<advNode>::iterator jt = it; jt != activeNode.end(); ++jt ) 
			{
				//find how many nodes use the same channel
				if(find(tmpChannel, jt -> getAbsoluteChannel())) 
				{
					usedBy++;
					colliding.push_back(*jt);
				}
			}
			
			//if more than one node use the same channel then manage collisions
			if(colliding.size() > 1) 
			{
				int size = colliding.size();
				//cout<<"size is: "<<size<<endl;
				int* genNumbers = new int[size];
				int i = 0;
				//cout<<"generatedNumbers:";
				
				//let each node generate a random number to handle collision
				for( list<advNode>::iterator at = colliding.begin(); at != colliding.end(); ++at ) {
					genNumbers[i] = at -> generateNumber(size, rand);
					//cout<<genNumbers[i]<<'\t';
					i++;
				}
				//cout<<endl;
				
				//erase all the list of colliding nodes to save resources
				colliding.erase(colliding.begin(), colliding.end());
				//cout<<"result is: "<<findMax(genNumbers, size);
				//see if the collision is solved or not
				if(findMax(genNumbers, size)) 
					collisionSolved = true;
				else 
				{
					collisionSolved = collisionSolved & false;
				}
				delete genNumbers;
			}
		}
	}
	tmpChannel.erase(tmpChannel.begin(), tmpChannel.end());
	//cout<<"collisionSolved? "<<collisionSolved<<endl;
	return collisionSolved;
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
int Timeslot::timeslotManager(int method)
{
	
	bool matchFound = false;
	char t;
	
	int timeslotOn = 0;
	/*
	* In the STAGGERED schema, a node becomes active at a certain point 
	* and it waits until the first EB is received correctly ALWAYS ON and
	* ON THE SAME CHANNEL 
	*/
	if(method == STAGGERED)
	{
		/*
		* N is the number of timeslots, listenerChannels is the number of channels
		* available to the listener
		*/
		timeslotOn = rand.getNumber( N * listenerChannels ) + 1;
		
		//cout<<"timeslotOn: "<<timeslotOn<<endl;
		
		//increment the asn, 
		while(asn < timeslotOn)
			asn++;
	}
	
	//until a match hasn't been found increment absolute sequence number and look for a match
	while(!matchFound) {
		
		//insert active nodes in the list of active nodes
		insertActive(asn);
		
		//print the list of active channels
		//cout<<"\t****AbsoluteSequenceNumber: "<<asn<<"****"<<endl;
		//print();
		
		/*
		 * In the fixed schema the listener becomes active at the first slot
		 * and remain active until a match is found
		 */
		bool collisionSolved = solveCollisions();
		
		/*
		* if listener and advertiser use the same channel and 
		* there are no collisions then the match is found
		*/
		if(compareChannel() && collisionSolved)  
		{
			matchFound = true;
		}
		eraseActive();
		asn++;
		
	}
	
	//if the method is fixed then the measure is performed in slotframe 
	if(method == FIXEDSCHEMA) 
	{
		int ratio = (asn / N) + 1;
		return ratio;
	}
		
	else
	{
		int ret = asn - timeslotOn;
		//cout<<"return value: "<<ret<<endl; 
		return ret;
	}
}
