#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>
#include <iostream>

#define maxSIZE 30
enum methods {NOTEON, CONTROL};
using namespace std;

class midiData{
	
private: 
int method;
int params[3];

public: 

midiData();
void setMidiData(int _method, int chanel, int note, int val);
void setMethod(int _method);
void setParams(int chanel, int note, int val);

int getMethod();
int* getParams();
	
	
};

class FIFOmidi{
	private:
	
	boost::circular_buffer<midiData> fifo;
	
	
	boost::mutex mtx_;
	boost::condition_variable cond;
	//bool full, empty;
	
	public:
	
	FIFOmidi();
	void lock();
	void unlock();
	
	void insert(midiData object);
	midiData get();
	
	
	
};
