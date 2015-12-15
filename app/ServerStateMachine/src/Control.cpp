#include "Control.hpp"
#include "Pushing.hpp"
#include "Sliding.hpp"

#define MACHINE context<Machine>()

using namespace std;
//Machine::Machine(){}

Machine::Machine(FIFOmidi& m_fifo):_midififo(m_fifo){}

Control::Control(){cout << "Init control"<<endl;}

Control::~Control(){cout << "Destroying Control"<< endl;}

Iddle::Iddle(){cout << "Starting iddle" << endl;}

Iddle::~Iddle(){cout << "Destroying iddle"<< endl;}

sc::result Iddle::react(const EvPress &){
	
	
		cout << "Pushing State"<< endl;
		if(MACHINE.ispause){
			MACHINE.ispause = false;
			
		}else{
			MACHINE.ispause = true;
			
		}
		if(!MACHINE.params.empty()){
		/*for(int i = 1; i< context<Machine>().params.size();++i){
			cout<< context<Machine>().params[i] 
		}*/
		MACHINE.midi.setMethod(context<Machine>().params[0]);
		MACHINE.midi.setParams(context<Machine>().params[1],context<Machine>().params[2],context<Machine>().params[3]);
		MACHINE._midififo.insert(context<Machine>().midi);
		cout<< "Midi Note ON!!!" << endl;
		}
		
		return transit<Pushing>();
	}
	
sc::result Iddle::react(const EvSlide &){
		    if(!MACHINE.params.empty()){
		/*for(int i = 1; i< context<Machine>().params.size();++i){
			cout<< context<Machine>().params[i] 
		}*/
		MACHINE.midi.setMethod(context<Machine>().params[0]);
		MACHINE.midi.setParams(context<Machine>().params[1],context<Machine>().params[2],context<Machine>().params[3]);
		MACHINE._midififo.insert(context<Machine>().midi);
		cout<< "Midi Slide!!!" << endl;
		}
		return transit<Sliding>();
	
	}
