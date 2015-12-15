#include "FIFOmididata.hpp"


midiData::midiData(){
}

void midiData::setMidiData(int _method, int chanel, int note, int val){
	 method =_method; 
	 params[0] = chanel;
	 params[1] = note;
	 params[2] = val;
}

void midiData::setMethod(int _method){
	method = _method; 
}

void midiData::setParams(int chanel, int note, int val){
	params[0] = chanel;
	 params[1] = note;
	 params[2] = val;
}

int midiData::getMethod(){
	return method;
}

int*  midiData::getParams(){
	return params;
}

FIFOmidi::FIFOmidi(){
	
	fifo.set_capacity(maxSIZE);
	
}
void FIFOmidi::lock(){
	mtx_.lock();

}

void FIFOmidi::unlock(){
	mtx_.unlock();
}

void FIFOmidi::insert(midiData object){
	boost::unique_lock<boost::mutex> lock(mtx_);
	if(!fifo.full()){
	fifo.push_back(object);
	cond.notify_one();
	}else{
	std::cout << "Fifo full!! Throwing object" << std::endl;		
	}
		
}

midiData FIFOmidi::get(){
	midiData ret;
	boost::unique_lock<boost::mutex> lock(mtx_);
	if(fifo.empty()){
		std::cout << "Fifo midi empty, waiting() ..." << std::endl;
		cond.wait(lock);
	}
	ret = fifo.front();
	fifo.pop_front();
	return ret;
}


//int main(int argc, char* argv[]){
	
	//midiData midi;
	
	
	//midi.setMidiData(NOTEON, 1, 2, 3);
	//std::cout << midi.getMethod() << "  ";
	//for(int i =0; i <  3; ++i){
		//std::cout << midi.getParams()[i] << ",";
	//}
	//std::cout << std::endl;
	
	//return 0;
	
//}

