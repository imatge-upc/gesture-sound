#include "Control.hpp"
#include "Sliding.hpp"
#include <iostream>

#define MACHINE context<Machine>()

using namespace std; 

Sliding::Sliding(){
	cout << "New Slider!!!"<< endl;
	
	}
	
Sliding::~Sliding(){cout << "Destroy Slider"<< endl;}

sc::result Sliding::react(const EvSlide &){
	    if(!MACHINE.params.empty()){
		/*for(int i = 1; i< context<Machine>().params.size();++i){
			cout<< context<Machine>().params[i] 
		}*/
		MACHINE.midi.setMethod(context<Machine>().params[0]);
		MACHINE.midi.setParams(context<Machine>().params[1],context<Machine>().params[2],context<Machine>().params[3]);
		MACHINE._midififo.insert(context<Machine>().midi);
		
		}
	cout << "MIDI Sliding!!"<< endl;
	return discard_event(); 
	
	}
