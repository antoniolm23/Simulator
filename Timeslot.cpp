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
 * From the list of all the neighbours nodes,
 * extract only the active nodes in that timeslot
 * @param: absolute sequence number
 */
void Timeslot::insertActive(int asn)
{
	int channelUsed;
	for( list<advNode>::iterator it = listenerNeighbours.begin(); 
		it != listenerNeighbours.end(); ++it  )
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

/**
 * searches for a match throughout the list of advertisers
 * Taking care of collisions too
 */ 
bool Timeslot::compareChannel(int timeslotOn)
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
		 * check if node is active, then check if the channel is correct
		 */
		if(it -> getTransmittingState())
		{
			if(it->getAbsoluteChannel() == listener.channelUsed)
			{
				//save IDs of transmitting nodes 
				idTransmitters.push_back(it -> getNodeID());
			}
		}
	}
	
	correctTransmission = idTransmitters.size();
	
	if(correctTransmission < 1)
		return false;
	
	if(correctTransmission == 1)
	{
		//cout<<"transmission"<<endl;
		return true;
	}
	else
	{
		return lookforCollision(idTransmitters);
	}
	
}

/**
 * If there is at least a node that is the only one in its communication range 
 * that transmitts than collision is solved, otherwise we have a collision
 * @return: true if the collision is solved, false otherwise
 */
bool Timeslot::lookforCollision(list<int> idTransmitters)
{
	//number of transmitting nodes
	int transmitters = 0;
	
	for( list<advNode>::iterator it = activeNode.begin(); it != activeNode.end(); ++it)
	{
		//if the node is active check in its communication range
		if(it -> getTransmittingState())
		{
			//cout << "ActiveNode: " << it -> getNodeID() << endl;
			//cout << "Neighbours: ";
			//for each possible id check if the node is in the neighborhood
			for(list<int>::iterator jt = idTransmitters.begin(); jt != idTransmitters.end(); ++jt)
			{
				//cout << *jt << '\t';
				if(it -> findIdNeighbour(*jt))
					transmitters++;
			}
			
			/**
			 * if there is just one transmitter in both communication ranges:
			 * listener and advertiser node, everything is ok
			 */ 
			if(transmitters == 1)
				return true;
		}
		
		//cout <<"\ttransmitters: "<<transmitters << endl;
		transmitters = 0;
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
	listNode.clear();
	eraseActive();
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
int Timeslot::timeslotManager(int m, int* transmittedEB)
{
	method = m;
	bool matchFound = false;
	asn = 0;
	
	int timeslotOn = 0;
	
	int tmpTransmittedEB = 0;
	*transmittedEB = 0;
	
	//for each node in the list checks how many colliders are there
	/*
	 * Even though in RandomHorizontal or RandomVertical 
	 * the collision probability is equal to 1/availablechannel,
	 * to have an effective comparison between the methods I 
	 * set the same collision probability for all the methods
	 */
	selectListenerNeighbours();
	
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
	while(!matchFound) {
		bool transmitterPresent = false;
		/*
		 * insert active nodes in the list of active nodes
		 * handles the ploss. In this case ploss is intended as the probability of 
		 * having a certain node active in its turn
		 */
		insertActive(asn);
		
		/*
		 * In every considered schemas the listener becomes active at a certain slot
		 * and remain active on the same channel until a match is found
		 */
		//bool collisionSolved = solveUniformCollisions();
		if(activeNode.size() > 0)
		{
			//cout<<"activeNodes\n";
			transmitterPresent = solveDifferentCollisions(&tmpTransmittedEB);
			*transmittedEB += tmpTransmittedEB;
			char t;
			//print();
		}
		
		/**
		 * if listener and advertiser use the same channel and 
		 * there are no collisions then the match is found
		 */
		if(transmitterPresent)
			if(compareChannel(asn))  
			{
				matchFound = true;
			}
		eraseActive();
		asn++;
		
	}
	
	eraseListenerNeighbours();
	
	//if the method is fixed then the measure is performed in slotframe 
	if(method == FIXEDSCHEMA) 
	{
		int ratio = (asn / N) + 1;
		return ratio;
	}
		
	//otherwise return the number of timeslot elapsed
	else
	{
		int ret = asn - timeslotOn + 1;
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
 * @return: if the listener can be in the selected position
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
 * @return: true if the listener can be in that position, false otherwise 
 */
bool Timeslot::allowableListener()
{
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		int dX = (it ->getPosX() - listener.xPos) * (it -> getPosX() - listener.xPos);
		int dY = (it ->getPosY() - listener.yPos) * (it -> getPosY() - listener.yPos);
		double distance = sqrt((dX) + dY );
		
		if(distance < transmissionRange && distance != 0)
			return true;
	}
	//cout<<"wrong pos\n";
	return false;
}

/**
 * Select the list of neighbours of the listener
 * In this way I save simulation time since I don't have to check every time every node
 */
void Timeslot::selectListenerNeighbours()
{
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++it)
	{
		int dX = (it ->getPosX() - listener.xPos) * (it -> getPosX() - listener.xPos);
		int dY = (it ->getPosY() - listener.yPos) * (it -> getPosY() - listener.yPos);
		double distance = sqrt((dX) + dY );
			
		if(distance < transmissionRange)
		{
			listenerNeighbours.push_back(*it);
			//cout << it -> getNodeID() << " " 
			//<< it -> getPosX() << "\t" << it -> getPosY() << endl;
		}
	}
}

/**
 * For each nodes, counts the neighbours and sets the collision probability
 * Set also the IDs of the possible colliding nodes
 */
void Timeslot::setNodesCollisionProbability()
{
	
	list<int> idColliders = list<int>();
	
	int collision = 0;
	for(list<advNode>::iterator it = listNode.begin(); it != listNode.end(); ++ it) 
	{
		for(list<advNode>::iterator jt = listNode.begin(); jt != listNode.end(); ++jt)
		{
			//computing distance between nodes
			int dX = (it ->getPosX() - jt -> getPosX()) * (it -> getPosX() - jt -> getPosX());
			int dY = (it ->getPosY() - jt -> getPosY()) * (it -> getPosY() - jt -> getPosY());
			double distance = sqrt((dX) + dY );
			
			//check if nodes may collide
			if(distance < transmissionRange)
			{
				collision++;
				idColliders.push_back(jt->getNodeID());
			}
		}
		it -> setColliders(collision);
		it -> setIdNeighbours(idColliders);
		//cout << "Collision: " << it -> getNodeID() << " " << it -> getColliders() << endl;
		collision = 0;
		idColliders.clear();
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
	
	//vector<int> usedChannels;
	bool verticalCollision = false;
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
			verticalCollision = it -> getVerticalState();
			double probCollision = it -> getVerticalCollision() / it -> getColliders();
			
			/**
			 * If there are more used cells than neighbours, so probCollision 
			 * is higher than one, fix the collisionProbability to 1 / verticalCells
			 * where vertical cells is the number of cells put in vertical
			 */
			if(probCollision >= 1)
				probCollision = 1 / it -> getVerticalCollision();
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
			int genNumb = it -> generateNumber(it -> getColliders(), rand);
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
	
	//cout << "end gen" << endl;
	
	//set the number of effectively transmitted EB
	*transmittedEB = transmittingNodes;
	
	if(transmittingNodes == 0)
	{
		return false;
	}
	
	else
		return true;
}

void Timeslot::eraseListenerNeighbours()
{
	listenerNeighbours.clear();
	//cout << "Size:" << neighbours.size() << endl;
}


