#include "Stats.h"

/**
 * Insert, if necessary, a statistic referred to a new iteration
 * @params: frequency, timeslotnumber
 */
void Stat::statInsert(statStruct s)
{
	//scan the list of statistics and if necessary increment the frequeny
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it) 
	{
		if(it->method == s.method)
			if(it->timeslotNumber == s.slotNumber)
			{
				it->frequency++;
				return;
			}
	}
	
	//if the scan wasn't successful add a new record in the list
	statElem st;
	st.frequency = 1;
	st.timeslotNumber = s.slotNumber;
	st.method = s.method;
	statsList.push_back(st);
}

/**
 * Function that computes the statistics on the list and prints it on the file
 * @param: the method used to perform the join
 * @return: the average number of steps required 
 */
double Stat::computeStatistic(int method) {
	
	double sum = 0;
	double ret;
	
	//cout<<"Method: "<<method<<endl;
	
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		if(it -> method == method)
		{
			sum += (it -> timeslotNumber * it->frequency);
			//cout<<it -> timeslotNumber<<' '<<it->frequency<<' '<<sum<<endl;
		}
	}
	
	ret = sum / iterations; 
	return ret;
}

//prints the list of statistics on file
void Stat::print()
{
	ofstream myfile;
	myfile.open("statistics.txt", ios::app);
	
	//computeStatistic();
	
	for(list<statElem>::iterator it = statsList.begin(); it != statsList.end(); ++it)
	{
		myfile<<"FrameslotNumber\t"<<it->timeslotNumber<<"\tfrequency:\t"<<it->frequency<<endl; 
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
	
	double tot = computeStatistic(method);
	myfile<<schema<<":\tMethod:\t"<<method<<"\tAvg:\t"<<tot<<endl;; 
	
	myfile.close();
}

//set the number of iterations performed
void Stat::setIterations(int it)
{
	iterations = it;
}


