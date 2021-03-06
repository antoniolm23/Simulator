#include "Timeslot.h"

/**
 * Constructor
 * @param: the random class, the transmission range, the listener channel 
 */
Timeslot::Timeslot(Random r, double ra, int lc)
{
	asn = 0; 
	rand = r; 
	ploss = 0;
	transmissionRange = ra;
	listenerChannels = lc;
}
/*
 * Functions to insert a node in one of the lists
 */
void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
	//cout<<"added node"<<a.getNodeID()<<"\n";
}

/**
 * From the list of all the nodes extract only the active nodes in that timeslot
 * @param: absolute sequence number
 */
void Timeslot::insertActive(int asn)
{
	int channelUsed;
	
	for( list<advNode>::iterator it = neighbours.begin(); it != neighbours.end(); ++it  )
	{
		//cout<<"ListNode: "<<it->getNodeID()<<": "<<it->getUsedChannel(asn)<<"\n";
		//if node active then add a record in the list
		if((channelUsed = it -> getUsedChannel(asn)) != -1) 
		{
			
			/*
			 * if there is a certain ploss of losing the message, a node can be inserted in 
			 * the list of active nodes according to that probability 
			 * i.e. a node is active with probability 1 - ploss
			 */
			if(method == PLOSS_SCENARIO)
			{
				double pActive = rand.getNumber01();
				//cout<<pActive<<" VS "<<ploss<<endl;
				if(pActive > ploss)
					activeNode.push_back(*it);
			}
			
			else 
				activeNode.push_back(*it);
			
			
			//cout<<"Active: "<<it->getNodeID()<<": "<<it->getUsedChannel(asn)<<"\n";
		}
	}
}

/**
 * adds the listener channel
 * @params: listener channel without ChannelStart
 */
void Timeslot::setListenerChannel(int l)
{
	listener.channelUsed = l + CHSTART;
}

//searches for a match throughout the list of advertisers
bool Timeslot::compareChannel(int timeslotOn)
{
	if(asn < timeslotOn)
	{
		return false;
		
	}
	
	//scan all the list looking for a match
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		if(it->getAbsoluteChannel() == listener.channelUsed)
			return true;
	}
	return false;
}

void Timeslot::print()
{
	cout<<listener.channelUsed<<endl;
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


/**
 * Function used when we use the tsch random number generator, to remember the 
 * fact that we have generated a number in the listNode too, in fact while 
 * activeNode is a temporary list, listNode is the definitive one.
 * @param: nodeId: the unique identifier for a node; size: the max dimension of the random number
 */
int Timeslot::getRandomNumber(int nodeId, int size) {
	
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it ) {
		if(it -> getNodeID() == nodeId)
			return it -> generateNumber(size, rand);
	}
	return -1;
}


//find if the max value available (size-1) is present in the list 
int findMax(int* v, int size) 
{
	//int max = size - 1;
	int max = 0;
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

/**
 * Try to solve a collision in the same timeslot if there are more thn one node that want to transmitt
 * @return: true -> collision solved, false collision or colliding nodes are off
 */
bool Timeslot::solveUniformCollisions()
{
	//cout<<"to solve collisons"<<endl;
	
	if(activeNode.size() < 2)
		return true;
	
	int usedBy = 0;
	list<int> tmpChannel;
	list<advNode> colliding;
	
	bool collisionSolved = true;
	
	//check if the same channel is used by more than one node
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		
		//if the channel is not already in the list then push it in the list
		if(!find(tmpChannel, it->getAbsoluteChannel())) {
			tmpChannel.push_back(it -> getAbsoluteChannel());
			for( list<advNode>::iterator jt = it; jt != activeNode.end() ; ++jt ) 
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
				//cout<<"generatedNumbers:\t";
				
				//let each node generate a random number to handle collision
				for( list<advNode>::iterator at = colliding.begin(); at != colliding.end(); ++at ) 
				{
					/*
					 * to use the tsch random number generator, the rand class has to be null
					 */
					genNumbers[i] = getRandomNumber(at -> getNodeID(), size);
					
					//cout<<genNumbers[i]<<'\t';
					i++;
				}
				//cout<<endl;
				
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
			//erase all the list of colliding nodes to save resources
			colliding.erase(colliding.begin(), colliding.end());
		}
	}
	tmpChannel.erase(tmpChannel.begin(), tmpChannel.end());
	//cout<<"collisionSolved? "<<collisionSolved<<endl;
	return collisionSolved;
}

/**
 * Function that operates the most operations on a timeslot:
 * 1) arranges links in the timeslot
 * 2) checks wether or not channels are equal 
 * 3) operates on a timeslot Base
 * 4) if method is fixed returns the number of frameslot elapsed
 * 
 * @params: method used
 * @return: framselot elapsed
 */
int Timeslot::timeslotManager(int m)
{
	method = m;
	bool matchFound = false;
	asn = 0;
	
	int timeslotOn = 0;
	
	//for each node in the list checks how many colliders are there
	setNodesCollisionProbability();
	selectNeighbours();
	
	/*
	* In the STAGGERED schema and in all other schemas, a node becomes active at a certain point 
	* and it waits until the first EB is received correctly ALWAYS ON and
	* ON THE SAME CHANNEL 
	*/
	if(method == STAGGERED || method == PLOSS_SCENARIO)
	{
		/*
		* N is the number of timeslots, listenerChannels is the number of channels
		* available to the listener
		*/
		timeslotOn = rand.getNumber( N * listenerChannels ) + 1;
		
		//cout<<"Staggered"<<endl;
		
		//increment the asn, 
		while(asn < timeslotOn)
			asn++;
	}
	
	//until a match hasn't been found increment absolute sequence number and look for a match
	while(!matchFound) {
		
		/*
		 * insert active nodes in the list of active nodes
		 * handles the ploss. In this case ploss is intended as the probability of 
		 * having a certain node active in its turn
		 */
		insertActive(asn);
		
		//print the list of active channels
		cout<<"\t****AbsoluteSequenceNumber: "<<asn<<"****"<<endl;
		print();
		
		/*
		 * In every considered schemas the listener becomes active at a certain slot
		 * and remain active on the same channel until a match is found
		 */
		//bool collisionSolved = solveUniformCollisions();
		bool collisionSolved = solveDifferentCollisions();
		/*
		* if listener and advertiser use the same channel and 
		* there are no collisions then the match is found
		*/
		if(compareChannel(timeslotOn) && collisionSolved)  
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
		
	//otherwise return the number of timeslot elapsed
	else
	{
		int ret = asn - timeslotOn;
		//cout<<"return value: "<<ret<<endl; 
		return ret;
	}
}

/**
 * The probability declares how much is the packet loss that we have in our transmission
 * By default it is 0
 * @param: probability of having a loss
 */
void Timeslot::setProbability(double p)
{
	ploss = p;
	//cout<<p<<":  "<<ploss<<endl;
}


/**
 * Function to set properties of the listener node
 * @param: listenerNode structure
 */
bool Timeslot::setListener(listenerNode l)
{
	listener.channelUsed = l.channelUsed;
	listener.xPos = l.xPos;
	listener.yPos = l.yPos;
	return allowableListener();
}

/**
 * function that checks if the listener node has at least one neighbour
 * then insert the neighbours in the list neighbours so that 
 * we reduce the number of nodes that need to be simulated 
 */
bool Timeslot::allowableListener()
{
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		if ( abs(listener.xPos - it -> getPosX()) < transmissionRange && 
			abs(listener.yPos - it -> getPosY() < transmissionRange) )
			return true;
	}
	return false;
}

/**
 * Select the list of neighbours of the listener
 * In this way I save simulation time since I don't have to check every time every node
 */
void Timeslot::selectNeighbours()
{
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		if(abs(listener.xPos - it -> getPosX()) < transmissionRange &&
			abs(listener.yPos - it -> getPosY() < transmissionRange))
		{
			neighbours.push_back(*it);
			cout << it -> getNodeID() << "" 
			<< it -> getPosX() << "\t" << it -> getPosY() << endl;
		}
	}
}

/**
 * For each nodes, counts the neighbours and sets the collision probability
 */
void Timeslot::setNodesCollisionProbability()
{
	int collision = 0;
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++ it) 
	{
		for(list<advNode>::iterator jt = listNode.begin(); jt != listNode.end(); ++jt)
		{
			if(abs(jt -> getPosX() - it -> getPosX()) < transmissionRange &&
				abs(jt -> getPosY() - it -> getPosY() < transmissionRange))
				collision++;
		}
		it -> setColliders(collision);
		cout << it -> getNodeID() << " " << it -> getColliders() << endl;
	}
}

/**
 * For each timeslot each node generates a number and has a certain probability of 
 * transmitting. If there are more than one node in charge of transmitting a collision occurs
 * @return: true if a collision doesn't occurs, false otherwise
 */
bool Timeslot::solveDifferentCollisions()
{
	int transmittingNodes = 0;
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		int genNumb = it -> generateNumber(it -> getColliders(), rand);
		if(genNumb == TRANSMISSIONFLAG)
			transmittingNodes++;
	}
	if(transmittingNodes == 1)
		return true;
	else
		return false;
	
	
}

