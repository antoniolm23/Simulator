#include <iostream>
#include <cstdlib>

#define f (int) 5

using namespace std;

int main() 
{
	int a = 5;
	int b = 6;
	int c = a < b;
	int d = b < a;
	c = a + f;
	cout<<"c: "<<c<<",d: "<<d<<endl;
	return 0;
}
