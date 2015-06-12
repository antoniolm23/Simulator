#include "schedule.h"
#include <math.h>
/*
 * the default channel on which we try to compute the scheduling
 * is the 0 one, by means of theory we know that the schema will 
 * be equal for all other channels
 */
#define PHYCHANNEL 0

/**
 * Constructor of a class schedule
 * @params: ts is the number of timeslots per slotframe
 * 			nc is the number of channels
 * 			na is the number of advertisign cells that can be used
 */
Schedule::Schedule(int ts, int nc, int na)
{
	scheduling = list<advLink>();
	cellInSuperframe = list<int>();
	cellInSuperframe.push_back(0);
	//occupy the first cell of the matrix
	advLink link;
	link.timeslot = link.channelOffset = 0;
	scheduling.push_back(link);
	
	totSlots = ts;
	totChannel = nc;
	totAdvertiser = na;
}

/*
 * Function to output the result
 */
ostream& operator<<(ostream& ios, const Schedule& s)
{
	
	list<advLink>::const_iterator it;
	for( it = s.scheduling.begin(); it != s.scheduling.end(); ++it) 
		ios<<"timeslot: "<< it -> timeslot<<"\tchannel: "<<it->channelOffset<<"\n";
	return ios;
}

/**
 * Given the absolute time at which a beacon has to be sent, 
 * compute the timeslot in the first slotframe at which
 * the beacon has to be sent
 * @params: the absolute time
 * @return: the timeslot in the first slotframe
 */
int Schedule::computeTimeslot(int absTime)
{
	return absTime % totSlots;
}

/**
 * Given the timeslot tries to compute the channelOffset
 * that generates a transmission on that physical channel
 * on that slot
 * @params: the absolute time
 * @return: the channelOffset
 */
int Schedule::computeChannelOffset(int absTime)
{
	for(int j = 0; j < totChannel; j++) 
		if(((absTime + j) % totChannel) == PHYCHANNEL)
			return j;
	return -1;
}

/**
 * If there are two possibilities in picking the distance,
 * this method computes the best one according to performances
 * @params: the smaller distance
 * @return: the best one
 */
/** 
 * NOTE:
 * It's computed only the numerator part of the sum, it's avoided also the division by 
 * 2 that must be performed to compute a sum via the Gauss method, it's possible 
 * to behave in this way since the denominator part is equal for both numbers
 * @params: step, remaining timeslots to distribute, advertiser cells to be used 
 */
int Schedule::computePerformance(int dist, int cycle, int adv) 
{
	
	int d = dist;
	int sumSmallDistance, sumLargeDistance;
	
	//this is the fisrt distance to be computed
	sumSmallDistance = (adv -1 ) * (d * (d + 1)) + 
						((cycle - (adv - 1) * d) * 
						(cycle - ((adv - 1) * d) + 1 ));
	
	d = dist + 1; 
	sumLargeDistance = (adv - 1) * (d * (d + 1)) + 
						((cycle - (adv - 1) * d) * 
						(cycle - ((adv - 1) * d) + 1 ));
	
	if(sumSmallDistance > sumLargeDistance )
		return d;
	else
		return dist;
	
}

/*
 * Function that effectively computes the scheduling,
 * filling a list of entries timeslot, channeloffset,
 * so that each node knows the timeslot to use and the 
 * channel offset to use
 */
list<advLink> Schedule::computeSchedule()
{
	bool restDivision = false;
	
	/* 
	 * through theory we know that the best schema is the 
	 * one with equi-spaced beacons
	 */
	int cycle = totSlots * totChannel;
	int distance = cycle / totAdvertiser;
	
	/*
	 * if the number of used adveritser is not a divisor of
	 * the number of timeslots forming the cycle, we have
	 * another possible distance, evaluate performances and then
	 * choose the best one
	 */ 
	int tmp;
	if((cycle % totAdvertiser) != 0)
	{
		tmp = distance;
		distance = computePerformance(tmp, cycle, totAdvertiser);
		restDivision = true;
	}
	
	advLink link;
	int step = distance;
	tmp = 0;
	cellInSuperframe.push_back(cycle - 1);
	
	/*
	 * For each cellInSuperframe compute the originating schema
	 */
	for(int i = 1; i < totAdvertiser; i++) 
	{
		//cout<<distance<<endl;
		cellInSuperframe.push_back(distance);
		if(restDivision) {
			tmp = step;
			//cout<<"originalCycle: "<<cycle<<'\t';
			cycle -= tmp;
			tmp = cycle / (totAdvertiser - i);
			step = computePerformance(tmp, cycle, totAdvertiser - i);
			//cout<<"stepSize: "<<step<<"\tcycle: "<<cycle<<'\t'<<tmp<<endl;
		}
		
		//compute the assigned timeslot
		link.timeslot = computeTimeslot(distance);
		
		//compute the assigned channel
		link.channelOffset = computeChannelOffset(distance);
		
		//insert the computed timeslot in the list
		scheduling.push_back(link);
		
		//go to the next absolute timeslot
		distance += step;
	}
	
	return scheduling;
	
}

void Schedule::setPloss(double p)
{
	ploss = p;
}


/*
 * Once an optimal schedule has been found evaluate it in order to know the lower bound
 * for the average joining time
 */
void Schedule::evaluate()
{
	cellInSuperframe.sort();
	list<int>::iterator it, jt, et;
	list<double> etalist = list<double>();
	double eta1 = 1;
	int i = 1;
	int gaussSum = 0;
	double sum = 0;
	double avg = 0;
	it = cellInSuperframe.begin();
	
	//BEGIN COMPUTE ETAi
	//compute eta1
	for( jt = it; ++jt != cellInSuperframe.end(); ) 
	{
		int prev, suc;
		prev = *it;
		suc = *(++it);
		sum += pow(ploss, i) * (suc - prev);
		//cout<<"prev and suc "<<prev<<'\t'<<suc<<'\t'<<sum<<endl;
		i++;
		jt = it;
	}
	int s = i - 1;
	eta1 = (1 + sum) / (1 - pow(ploss, s));
	//cout<<e1<<'\t'<<s<<endl;
	etalist.push_back(eta1);
	
	//now we have to compute all the other ei starting from the last one and going backwards
	it = cellInSuperframe.end();
	--it;
	//cout<<"it: "<<*it<<'\t'<<*ti.end()<<'\t'<<*ti.begin()<<'\t'<<ti.back()<<endl;
	for(jt = it; --jt != cellInSuperframe.begin();)
	{
		int prev, suc;
		suc = *it;
		prev = *(--it);
		//cout<<suc<<'\t'<<prev<<'\t'<<ei.back()<<endl;
		if(etalist.size() == 1)
			sum = 1 + ploss * (suc - prev + etalist.back());
		else
			sum = 1 + ploss * (suc - prev - 1 + etalist.back());
		etalist.push_back(sum);
		//cout<<"Sum: "<<sum<<endl;
	}
	
	//END COMPUTE ETAi
	
	//BEGIN COMPUTE SUMS
	sum = 0;
	//now that we have computed all the various ei, we have to compute the effective avg
	it = cellInSuperframe.begin();
	//*(cellInSuperframe.end()) ++;
	
	//we need to add one to the last element in the list
	//FIXME find a better way to do this
	et = cellInSuperframe.end();
	--et;
	cellInSuperframe.pop_back();
	int tmp = *et;
	tmp++;
	cellInSuperframe.push_back(tmp);
	et = cellInSuperframe.end();
	--et;
	//<<'\t'<<*et<<endl;
	
	//compute the final sums
	for( jt = it; ++jt != cellInSuperframe.end(); ) 
	{
		int prev, suc;
		prev = *it;
		suc = *(++it);
		
		gaussSum = (suc - prev - 1) * (suc - prev) / 2;
		suc = *et;
		prev = *(--et);
		sum += (suc - prev) * etalist.front();
		etalist.pop_front();
		
		sum = sum + gaussSum;
		
		jt = it;
	}
	//END COMPUTE SUMS
	
	//cout<<"Fploss"<<ploss<<":\t"<<sum/tmp<<endl;
}

