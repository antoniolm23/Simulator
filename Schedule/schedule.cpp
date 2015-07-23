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
schedule::schedule(int ts, int nc, int na)
{
	scheduling = list<timeslot>();
	ti = list<int>();
	ti.push_back(0);
	//occupy the first cell of the matrix
	timeslot t;
	t.ts = t.nc = 0;
	scheduling.push_back(t);
	
	totSlots = ts;
	totChannel = nc;
	totAdvertiser = na;
}

/*
 * Function to output the result
 */
ostream& operator<<(ostream& ios, const schedule& s)
{
	list<timeslot> l = s.scheduling;
	list<timeslot>::iterator it;
	//for( it = l.begin(); it != l.end(); ++it) 
		//ios<<"timeslot: "<< it -> ts<<"\tchannel: "<<it->nc<<"\n";
	return ios;
}

/**
 * Given the absolute time at which a beacon has to be sent, 
 * compute the timeslot in the first slotframe at which
 * the beacon has to be sent
 * @params: the absolute time
 * @return: the timeslot in the first slotframe
 */
int schedule::computeTimeslot(int ti)
{
	cout<<"Timselot"<<ti % totSlots<<endl;
	return ti % totSlots;
}

/**
 * Given the timeslot tries to compute the channelOffset
 * that generates a transmission on that physical channel
 * on that slot
 * @params: the absolute time
 * @return: the channelOffset
 */
int schedule::computeChannelOffset(int ti)
{
	for(int j = 0; j < totChannel; j++) 
		if(((ti + j) % totChannel) == PHYCHANNEL)
		{
			//cout<<"Channel Offset"<<j<<endl;
			return j;
		}
	return -1;
}

/*
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
int schedule::computePerformance(int dist, int cycle, int adv) {
	
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
void schedule::computeSchedule()
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
	
	timeslot t;
	int step = distance;
	tmp = 0;
	ti.push_back(cycle - 1);
	/*
	 * For each ti compute the originating schema
	 */
	for(int i = 1; i < totAdvertiser; i++) 
	{
		//cout<<distance<<endl;
		ti.push_back(distance);
		if(restDivision) {
			tmp = step;
			//cout<<"originalCycle: "<<cycle<<'\t';
			cycle -= tmp;
			tmp = cycle / (totAdvertiser - i);
			step = computePerformance(tmp, cycle, totAdvertiser - i);
			//cout<<"stepSize: "<<step<<"\tcycle: "<<cycle<<'\t'<<tmp<<endl;
		}
		
		//compute the assigned timeslot
		t.ts = computeTimeslot(distance);
		
		//compute the assigned channel
		t.nc = computeChannelOffset(distance);
		
		cout<<"Channel: "<<t.nc<<"\tTimeslot: "<<t.ts<<endl;
		//insert the computed timeslot in the list
		scheduling.push_back(t);
		
		//go to the next absolute timeslot
		distance += step;
	}
	
	//return scheduling;
	
}

void schedule::setPloss(double p)
{
	ploss = p;
}


/*
 * Once an optimal schedule has been found evaluate it in order to know the lower bound
 * for the average joining time
 */
void schedule::evaluate()
{
	ti.sort();
	list<int>::iterator it, jt, et;
	list<double> ei = list<double>();
	double e1 = 1;
	int i = 1;
	int gaussSum = 0;
	double sum = 0;
	double avg = 0;
	it = ti.begin();
	
	//compute the first e1
	for( jt = it; ++jt != ti.end(); ) 
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
	e1 = (1 + sum) / (1 - pow(ploss, s));
	//cout<<e1<<'\t'<<s<<endl;
	ei.push_back(e1);
	
	//now we have to compute all the other ei starting from the last one and going backwards
	it = ti.end();
	--it;
	//cout<<"it: "<<*it<<'\t'<<*ti.end()<<'\t'<<*ti.begin()<<'\t'<<ti.back()<<endl;
	for(jt = it; --jt != ti.begin();)
	{
		int prev, suc;
		suc = *it;
		prev = *(--it);
		//cout<<suc<<'\t'<<prev<<'\t'<<ei.back()<<endl;
		if(ei.size() == 1)
			sum = 1 + ploss * (suc - prev + ei.back());
		else
			sum = 1 + ploss * (suc - prev - 1 + ei.back());
		ei.push_back(sum);
		//cout<<"Sum: "<<sum<<endl;
	}
	
	sum = 0;
	//now that we have computed all the various ei, we have to compute the effective avg
	it = ti.begin();
	*(ti.end()) ++;
	
	//increment the cycle variable
	et = ti.end();
	--et;
	ti.pop_back();
	int tmp = *et;
	tmp++;
	ti.push_back(tmp);
	et = ti.end();
	--et;
	//<<'\t'<<*et<<endl;
	
	//compute the final sums
	for( jt = it; ++jt != ti.end(); ) 
	{
		int prev, suc;
		prev = *it;
		suc = *(++it);
		
		gaussSum = (suc - prev - 1) * (suc - prev) / 2;
		suc = *et;
		prev = *(--et);
		sum += (suc - prev) * ei.front();
		ei.pop_front();
		
		sum = sum + gaussSum;
		
		jt = it;
	}
	cout<<"Fploss"<<ploss<<":\t"<<sum/tmp<<endl;
}

/**
 * With a file configuration part
 * @param: t is the filename
 */
schedule::schedule(string t, int ts, int ch)
{
	filename = t;
	totSlots = ts;
	totChannel = ch;
}

void schedule::retrieveSchedule()
{
	ifstream file;
	file.open("conf.txt");
	string line;
	string distanceString;
	int distanceInt, step = 0;
	int timeslot, channelOffset;
	//while there are lines in the file, read it and compute timeslot and channeloffset
	while(getline(file, distanceString))
	{
		
		//std::stringstream iss(line);
		
		
		//if(getline(iss, distanceString, '\n'))
		//{
			distanceInt = atoi(distanceString.c_str());
			cout<<distanceInt<<endl;
			step += distanceInt;
			cout<<step<<endl;
			timeslot = computeTimeslot(step);
			cout<<"timeslot: "<<timeslot<<endl;
			channelOffset = computeChannelOffset(step);
			cout<<"channel: "<<channelOffset<<endl;
			saveOnFile(channelOffset, timeslot);
			
			cout<<timeslot<<'\t'<<channelOffset<<endl;
		//}
	}
}

/**
 * Writes both integers on file
 * @param: integers to write
 */
void schedule::saveOnFile(int c, int t)
{
	ofstream file;
	file.open("../conf.txt", ios::app);
	file<<"0\t"<<c<<'\t'<<t<<'\n';
	file.close();
}


