#include "Stats.h"

/**
 * Insert, if necessary, a statistic referred to a new iteration
 * @params: frequency, timeslotnumber
 */
void Stat::statInsert(statStruct s)
{
	/*//scan the list of statistics and if necessary increment the frequeny
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it) 
	{
		if(it->method == s.method)
			if(it->timeslotNumber == s.slotNumber)
			{
				it->frequency++;
				return;
			}
	}*/
	
	//if the scan wasn't successful add a new record in the list
	/*statElem st;
	st.frequency = 1;
	st.timeslotNumber = s.slotNumber;
	st.method = s.method;
	st.EBsent = s.EBsent;*/
	statsList.push_back(s);
}

/**
 * Function that computes the slotNumber needed on average to 
 * compute the join 
 * @param: the method used to perform the join
 * @return: the average number of steps required 
 */
double Stat::computeMeanSlotNumber(int method) 
{
	double slotSum = 0;
	double ret;
	
	//cout<<"Method: "<<method<<endl;
	
	for(list<statStruct>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		if(it -> method == method)
		{
			slotSum += (it -> slotNumber);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
		}
	}
	
	ret = slotSum / TSiterations; 
	return ret;
}

/**
 * Function that computes the EB sent on average to compute the join
 * @param: the method used to perform the join
 * @return: the average number of EBsent 
 */
double Stat::computeMeanEBsent(int method)
{
	double EBsentSum = 0;
	double ret;
	
	//cout<<"Method: "<<method<<endl;
	
	for(list<statStruct>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		if(it -> method == method)
		{
			EBsentSum += (it -> EBsent);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
		}
	}
	
	ret = EBsentSum / EBiterations; 
	return ret;
}
//prints the list of statistics on file
void Stat::print()
{
	ofstream myfile;
	myfile.open("tmp/statistics.txt", ios::app);
	
	//computeStatistic();
	
	for(list<statStruct>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		myfile<<"FrameslotNumber\t"<<it->slotNumber;//<<"\tfrequency:\t"<<it->frequency<<endl; 
	}
	
	
	myfile.close();
}

/**
 * print the statistic on the file, 
 * each statistic is preceeded by the string t
 * @param: the name of the schema, the method used
 */
void Stat::print(string schema, int method)
{
	//cout<<"othermethodCalled"<<endl;
	ofstream myfile;
	myfile.open("statistics.txt", ios::app);
	
	double meanSlotNumber = computeMeanSlotNumber(method);
	double confidenceIntervalSlotNumber = computeConfidenceIntervalSlotNumber(method, meanSlotNumber);
	
	double meanEBsent = computeMeanEBsent(method);
	double confidenceIntervalEBsent = computeConfidenceIntervalEBsent(method, meanEBsent);
	myfile<<schema<<":\tMethod:\t"<<method<<"\tAvg:\t"<<meanSlotNumber
		<<"\tCI:\t"<<meanSlotNumber + confidenceIntervalSlotNumber<<'\t'
		<<meanSlotNumber - confidenceIntervalSlotNumber
		<<"\tAVGEB:\t"<<meanEBsent<<"\tCIEBsent:\t"<<
		meanEBsent + confidenceIntervalEBsent<<'\t'<<
		meanEBsent - confidenceIntervalEBsent<<endl; 
	
	myfile.close();
}

/**
 * Computes the 95% confidence interval for the average slot number required
 * @param: method used to compute the average joining time, average slot number required
 * @return: 95% confidence interval
 */
double Stat::computeConfidenceIntervalSlotNumber(int method, double mean) 
{
	double stDevSum = 0;
	for(list<statStruct>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		if(it -> method == method)
		{
			stDevSum += pow(((mean - it -> slotNumber)), 2);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
		}
	}
	double stDev = sqrt(stDevSum / TSiterations);
	double confidenceInterval = CONFIDENCE95 * (stDev / sqrt(TSiterations));
	
	return confidenceInterval;
}

/**
 * Computes the 95% confidence interval for the average EB sent
 * @param: method used to compute the average joining time, average EB sent
 * @return: 95% confidence interval
 */
double Stat::computeConfidenceIntervalEBsent(int method, double mean) 
{
	cout<<mean<<endl;
	double stDevSum = 0;
	for(list<statStruct>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		if(it -> method == method)
		{
			stDevSum += pow(((mean - it -> EBsent)), 2);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
		}
	}
	cout<<stDevSum<<endl;
	double stDev = sqrt(stDevSum / EBiterations);
	cout<<stDev<<endl;
	double confidenceInterval = CONFIDENCE95 * (stDev / sqrt(EBiterations));
	cout <<" Confidence Interval: "<<confidenceInterval << endl;
	return confidenceInterval;
}

