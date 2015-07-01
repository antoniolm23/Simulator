#include "Timeslot.h"

/**
 * Given cardinal position checks if two motes are or not neighbours
 * basing on the transmissionRange
 * @param: cardinal position of node a and of node b, the transmission range
 * @return: possible constant: OCCUPIED, INTXRANGE, AVAILABLEPOS 
 */
int checkNeighbours(double aPosX, double aPosY, double bPosX, 
					double bPosY, int transmissionRange)
{
	double dX = (aPosX - bPosX) * (aPosX - bPosX);
	double dY = (aPosY - bPosY) * ( aPosY - bPosY);
	double distance = sqrt((dX) + dY );
	//cout<<distance<<endl;
	if(distance < transmissionRange && distance != 0)
		return INTXRANGE;
	if(distance != 0)
		return AVAILABLEPOS;
	if(distance == 0)
		return OCCUPIED;
	return ERROR;
}

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
	printProb = false;
	listenersList = list<listenerNode>();
	firstIt = false;
}
/*
 * Functions to insert a node in one of the lists
 * NOTE: checking the correctness of the position is not necessary since this operation
 * is performed in the main
 */
void Timeslot::addNode(advNode a)
{
	listNode.push_back(a);
	//cout<<"added node"<<a.getNodeID()<<"\n";
}

/**
 * From the list of all nodes, extract only the active nodes in that timeslot
 * @param: absolute sequence number
 */
void Timeslot::insertActive(int asn)
{
	int channelUsed;
	for( list<advNode>::iterator it = listNode.begin(); 
		it != listNode.end(); ++it  )
	{
		//cout<<"ListNode: "<<it->getNodeID()<<": "<<it->getUsedChannel(asn)<<"\n";
		//if node active then add a record in the list
		if((channelUsed = it -> getUsedChannel(asn, method)) != -1) 
		{
			
			/*
			 * if there is a certain ploss of losing the message, a node can be inserted in 
			 * the list of active nodes according to that probability 
			 * i.e. a node is active with probability 1 - ploss
			 */
			if(method == PLOSS_SCENARIO || ploss != 0) 
			{
				//cout<<"ploss"<<endl;
				double pActive = rand.getNumber01();
				//cout<<pActive<<" VS "<<ploss<<endl;
				if(pActive > ploss)
					activeNode.push_back(*it);
			}
			
			else 
				activeNode.push_back(*it);
			//cout<<"Active: "<<it->getNodeID()<<"\n";
		}
	}
}

/**
 * searches for a match throughout the list of advertisers
 * Taking care of collisions too
 */ 
bool Timeslot::compareChannel(int timeslotOn, listenerNode listener)
{
	
	int correctTransmission = 0;
	if(asn < timeslotOn)
	{
		return false;
		
	}
	list<int> idTransmitters = list<int>();
	//scan all the list looking for a match
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		//cout << it -> getNodeID() <<" ABS ch: " << it -> getAbsoluteChannel() << endl;
		/**
		 * check if node is active and the listener is in its transmitting range
		 * then check if the channel is correct
		 */
		if(it -> getTransmittingState() && (
			checkNeighbours(it->getPosX(), it->getPosY(), 
			listener.xPos, listener.yPos, transmissionRange) == INTXRANGE))
		{
			//cout<<it->getAbsoluteChannel()<<'\t'<<listener.channelUsed<<endl;
			if(it->getAbsoluteChannel() == listener.channelUsed)
			{
				correctTransmission++;
			}
		}
	}
	
	if(correctTransmission == 1)
	{
		//cout<<"transmission"<<endl;
		return true;
	}
	else
	{
		return false;
	}
	
}

void Timeslot::print()
{
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it  )
	{
		cout<<'\t'<<it->getNodeID()<<' '<<it->getAbsoluteChannel();
	}
	cout<<endl<<"**************************************"<<endl;
}

//delete all the list
void Timeslot::erase()
{
	listNode.clear();
	eraseActive();
	listenersList.clear();
}

void Timeslot::eraseActive()
{
	activeNode.clear();
}


/**
 * Function used when we use the TSCH Random Number Generator, to remember the 
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
 * Try to solve a collision in the same timeslot if there are more then 
 * one node that want to transmitt
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
int Timeslot::timeslotManager(int m, double* transmittedEB)
{
	unsigned int joinedSlotSum = 0;
	
	method = m;
	unsigned int matchFound = 0;
	asn = 0;
	
	int timeslotOn = 0;
	
	int tmpTransmittedEB = 0;
	*transmittedEB = 0;
	
	/**
	 * each listener selects its channel
	 */
	for(list<listenerNode>::iterator it = listenersList.begin(); 
		it != listenersList.end(); ++it)
	{
		it -> channelUsed = rand.getNumber(listenerChannels) + CHSTART;
	}
	
	unsigned int totListeners = listenersList.size();
	list<listenerNode> tmpListenersList = list<listenerNode>(listenersList);
	if(firstIt == false)
		selectListenersNeighbours();
	
	//for each node in the list checks how many colliders are there
	
	/*
	* In the OPTIMUM schema and in all other schemas, a node becomes active at a certain point 
	* and it waits until the first EB is received correctly ALWAYS ON and
	* ON THE SAME CHANNEL 
	*/
	if(method != FIXEDSCHEMA)
	{
		/**
		 * N is the number of timeslots, listenerChannels is the number of channels
		 * available to the listener
		 */
		timeslotOn = rand.getNumber( N * listenerChannels ) + 1;
		
		//increment the asn, 
		asn = timeslotOn;
	}
	
	//until a match hasn't been found increment absolute sequence number and look for a match
	while(matchFound < totListeners) {
		
		bool transmitterPresent = false;
		/*
		 * insert active nodes in the list of active nodes
		 * handles the ploss. In this case ploss is intended as the probability of 
		 * having a certain node active in its turn
		 */
		insertActive(asn);
		
		//if(asn%101 == 0)
		//	cout<<asn<<" method: "<<method<<endl;
		
		/*
		 * In every considered schemas the listener becomes active at a certain slot
		 * and remain active on the same channel until a match is found
		 */
		//bool collisionSolved = solveUniformCollisions();
		if(activeNode.size() > 0)
		{
			transmitterPresent = solveDifferentCollisions(&tmpTransmittedEB);
			*transmittedEB += tmpTransmittedEB;
			//print();
		}
		
		/**
		 * if there is at least a transmitter, check if it uses the same channel of
		 * one of the listeners 
		 */
		if(transmitterPresent)
		{
			for(list<listenerNode>::iterator it = tmpListenersList.begin(); 
				it != tmpListenersList.end(); ++it)
			if(compareChannel(asn, *it))  
			{
				matchFound++;
				//throw out of the list the already joined node
				it = tmpListenersList.erase(it);
				
				//insert statistic
				joinedSlotSum += asn + 1 - timeslotOn + 1;
				//cout << "joinedSum "<<joinedSlotSum<<endl ;
			}
		}
		eraseActive();
		asn++;
	}
	double slotframeElapsed = asn - timeslotOn;
	//cout<<asn<<'\t'<<timeslotOn<<'\t'<<slotframeElapsed<<'\t'<<*transmittedEB<<endl;
	slotframeElapsed = slotframeElapsed / N;	
	*transmittedEB = *transmittedEB / slotframeElapsed;
	//cout<< *transmittedEB << endl;
	return joinedSlotSum;
	
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
 * @return: if the listener can be in the selected position
 * NOTE: control performed outside
 */
bool Timeslot::addListener(listenerNode l)
{
	firstIt = false;
	if(allowableListener(l) == true)
	{
		listenersList.push_back(l);
		return true;
	}
	else
		return false;
}

/**
 * function that checks if the listener node has at least one neighbour
 * then insert the neighbours in the list neighbours so that 
 * we reduce the number of nodes that need to be simulated
 * @param: listenerNode structure
 * @return: true if the listener can be in that position, false otherwise 
 */
bool Timeslot::allowableListener(listenerNode listener)
{
	bool okPosition = false;
	bool inTransmissionRange = false;
	int result;
	
	//check compatibility with advertiser nodes
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		result = checkNeighbours(listener.xPos, listener.yPos, it -> getPosX(), 
								 it -> getPosY(), transmissionRange);
		if(result == INTXRANGE)
		{
			inTransmissionRange = true;
			okPosition = true;
		}
		if(result == AVAILABLEPOS)
			okPosition = true;
		if(result == OCCUPIED)
			return false;
		
	}
	
	//check compatibility with other listener nodes
	for(list<listenerNode>::iterator it = listenersList.begin(); 
		it != listenersList.end(); ++it)
	{
		result = checkNeighbours(listener.xPos, listener.yPos, it -> xPos, 
								 it -> yPos, transmissionRange);
		if(result == INTXRANGE || AVAILABLEPOS)
			okPosition = true;
		if(result == OCCUPIED)
			return false;
	}
	
	if(okPosition == true && inTransmissionRange == true)
	{
		return true;
	}
	else
		return false;
}

/**
 * For each nodes, counts the neighbours and sets the collision probability
 * Set also the IDs of the possible colliding nodes
 */
void Timeslot::setNodesCollisionProbability()
{
	int result;
	int collision = 0;
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++ it) 
	{
		for(list<advNode>::iterator jt = listNode.begin(); jt != listNode.end(); ++jt)
		{
			//computing distance between nodes
			result = checkNeighbours(it -> getPosX(), it -> getPosY(), 
				jt->getPosX(), jt->getPosY(), transmissionRange);
			if(result == INTXRANGE || result == OCCUPIED)
				collision++;
		}
		it -> setColliders(collision);
		//cout << "Collision: " << it -> getNodeID() << " " << it -> getColliders() << endl;
		collision = 0;
	}
}

/**
 * For each timeslot each node generates a number and has a certain probability of 
 * transmitting. If there are more than one node in charge of transmitting a collision occurs
 * Furthermore this function enables effective transmitting nodes
 * @return: true if a collision doesn't occurs, false otherwise
 */
bool Timeslot::solveDifferentCollisions(int* transmittedEB)
{
	
	int transmittingNodes = 0;
	
	int genNumb = 0; 
	//vector<int> usedChannels;
	//cout << activeNode.size() << endl;
	for(list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it)
	{
		/**
		 * in case we use the OPTIMUM SCHEMA and we have the vertical case, 
		 * change collision probability accordingly, 
		 * the collisionProbability in this case is
		 * N_cells / N_neighbours
		 */
		if(it -> getVerticalState() && method == OPTIMUM) 
		{
			double probCollision = it -> getVerticalCollision() / it -> getColliders();
			
			probCollision = probCollision / energyFactor; 
			
			/**
			 * If there are more used cells than neighbours, so probCollision 
			 * is higher than one, fix the collisionProbability a constant, if this
			 * number is higher than one this means that we have a very sparse topology,
			 * to avoid locks, the constant is very high
			 */
			double generatedNumber01 = it -> generateNumber01(rand);
			//cout << probCollision << "\t" << generatedNumber01 << endl;
			if(generatedNumber01 < probCollision)
			{
				transmittingNodes++;
				it -> setTransmittingState(true);
			}
			//even though the node is active, it dosen't transmit in this timeslot 
			else
			{
				it -> setTransmittingState(false);
			}
			
			//usedChannels.push_back(it -> getAbsoluteChannel());
		}
		
		/**
		 * handle normal probability, when transmission probability
		 * is related to the number of neighbours
		 */
		else
		{
			if(method == OPTIMUM)
			{
				/**
				 * Control to avoid an always transmitting node.
				 * NOTE: TRANSMISSIONFLAG is equal to 0, that's why this works
				 */
				/*if(it -> getColliders() == 1)
				{
					if(it->generateNumber01(rand) / energyFactor < HIGHERPROB)
					{
						genNumb = TRANSMISSIONFLAG;
					}
					else
					{
						genNumb = TRANSMISSIONFLAG + 1;
					}
				}
				else*/
				genNumb = it -> generateNumber(it -> getColliders() * energyFactor, rand);
			}
		
			else
				genNumb = it -> generateNumber(energyFactor, rand);
			//cout << it -> getNodeID() << ": " << genNumb <<endl;
			if(genNumb == (int)TRANSMISSIONFLAG)
			{
				it -> setTransmittingState(true);
				transmittingNodes++;
				/*if(method == RANDOMVERTICAL)
				{
					//usedChannels.push_back(it->getAbsoluteChannel());
				}*/
			}
			else
				it -> setTransmittingState(false);
		}
		
	}
	
	printProb = true;
	//set the number of effectively transmitted EB
	*transmittedEB = transmittingNodes;
	
	if(transmittingNodes == 0)
	{
		return false;
	}
	
	else
		return true;
}

/**
 * For each listener counts the number of its neighbours
 */
void Timeslot::selectListenersNeighbours()
{
	firstIt = true;
	for(list<listenerNode>::iterator it = listenersList.begin(); 
		it != listenersList.end(); ++it)
	{
		int neighbours = 0;
		for(list<advNode>::iterator jt = listNode.begin(); jt != listNode.end(); ++jt)
		{
			int result = checkNeighbours(it->xPos, it->yPos, jt->getPosX(), 
										 jt->getPosY(), transmissionRange);
			if(result == INTXRANGE)
				neighbours++;
		}
#ifdef DEBUG 
		cout<<"Neighbours:\t"<<neighbours<<endl;
#endif
	}
}
