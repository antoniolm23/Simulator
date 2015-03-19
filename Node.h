#include <iostream>
#include <cstdlib>
using namespace std;

//this class defines an advertiser Node
class advNode {
	int usedChannel;	//channel used to advertise
	int availableChannels;	//channels available
	bool state;			//state of the advertiser node (active or not)
	
public:
	advNode(int uc, int tc);
	int getUsedChannel();
	void setUsedChannel(int uc);
	bool getState();
	void setState(bool s);
	void print(ostream& );
	void changeChannel();
};