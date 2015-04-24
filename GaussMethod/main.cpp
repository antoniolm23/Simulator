#include <iostream>
#include <cstdlib>

using namespace std;

int main() 
{
	int nt;
	cout<<"Insert number of ti: ";
	cin>>nt;
	cout<<endl;
	int oldT, newT;
	oldT = 0;
	int sum = 0;
	for(int i = 0; i < nt; i++)
	{	
		cin>>newT;
		sum = sum + (newT - oldT) * (newT - oldT + 1) / 2;
		oldT = newT;
	}
	cout<<sum<<endl;
	return 0;
}
