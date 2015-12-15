#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>
#include "d3message.hpp" 

#define maxSIZE 30
using namespace std;

class FIFOd3data{
	private:
	
	boost::circular_buffer<d3data> fifo;
	
	
	boost::mutex mtx_;
	boost::condition_variable cond;
	//bool full, empty;
	
	public:
	
	FIFOd3data();
	void lock();
	void unlock();
	
	void insert(d3data object);
	d3data get();
	//bool isEmpty();
	//bool isFull();
	

	
};
