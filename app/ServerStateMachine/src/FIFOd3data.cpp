#include <boost/date_time/posix_time/posix_time.hpp>
#include "FIFOd3data.hpp"


FIFOd3data::FIFOd3data(){
	
	fifo.set_capacity(maxSIZE);
	
}
void FIFOd3data::lock(){
	mtx_.lock();

}

void FIFOd3data::unlock(){
	mtx_.unlock();
}

void FIFOd3data::insert(d3data object){
	boost::unique_lock<boost::mutex> lock(mtx_);
	if(!fifo.full()){
	//std::cout << "FIFO IN!!" << std::endl;			
	fifo.push_back(object);
	cond.notify_one();
	}else{
	std::cout << "Fifo full!! Throwing object" << std::endl;		
	}
		
}

d3data FIFOd3data::get(){
	d3data ret;
	boost::unique_lock<boost::mutex> lock(mtx_);
	if(fifo.empty()){
		//std::cout << "Fifo empty, waiting() ..." << std::endl;
		cond.wait(lock);
	}
	ret = fifo.front();
	fifo.pop_front();
	return ret;
}

//class T1{
	
	//private:
	//FIFOd3data& d1;
	//public:
	//T1(FIFOd3data& data): d1(data){}
	
	//void operator ()(){
		
		//while (1){
			////d1.insert(1);
			////boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
			
			//std::cout << d1.get().toString() << std::endl;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			
			
		//}
		
	//}
//};



//int main(int argc, char** argv){
	
	//FIFOd3data fifo;
	//d3message d3; 
	//std::string str = "{\"d3joint\":[-0.64118207,-0.33531687,1.2718775,0.024604935,-0.2777965,1.3148462],\"errors\":[],\"h_st\":[1,3],\"n_joint\":2,\"static\":-1}";
	//d3.strToObject(str);
	//T1 t(fifo);
	//boost::thread t1(boost::ref(t));
	//while(1){
		
		//fifo.insert(d3.getd3data());
		//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	//}
	
	//return 0;
	
	
	
//}
