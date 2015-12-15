#include "StateMachine.hpp"



#define maxY 0.70
#define step 0.0055
StateMachine::StateMachine(FIFOd3data& d_fifo, FIFOmidi& m_fifo ):myMachine(m_fifo),_sharedfifo(d_fifo){

}

void StateMachine::process(){
		d3data data;
		std::vector<int> ref;
		data = _sharedfifo.get();
		//std::cout << data.toString() << std::endl;
		/*if(data.get_h_st().at(0)==3&&isOpen){
			std::cout << "NOTE ON!!\n" << std::endl;
			//playpause.process_event(EvStart());
			//playpause.process_event(EvGetMidi(ref));
			midi.setMethod(NOTEON);
			midi.setParams(ref[0],ref[1],ref[2]);
			_midififo.insert(midi);
			isOpen = false;
		}else{
			//playpause.process_event(EvStop());
		}
		*/
		
		if(data.get_d3joints().at(0).x < -0.8){
			 if(myMachine.ispause){
			    myMachine.params.push_back(NOTEON);
				myMachine.params.push_back(0);
				myMachine.params.push_back(0);
				myMachine.params.push_back(1);
				myMachine.process_event(EvPress());
				myMachine.params.resize(0);
				
			}else{
				myMachine.params.push_back(NOTEON);
				myMachine.params.push_back(0);
				myMachine.params.push_back(1);
				myMachine.params.push_back(1);
				myMachine.process_event(EvPress());
				myMachine.params.resize(0);
				
			}
		}else if(data.get_d3joints().at(1).x > 0.6){
			if(myMachine.ispause){
			myMachine.params.push_back(NOTEON);
				myMachine.params.push_back(0);
				myMachine.params.push_back(2);
				myMachine.params.push_back(1);
				myMachine.process_event(EvPress());
				myMachine.params.resize(0);
				
			}else{
				myMachine.params.push_back(NOTEON);
				myMachine.params.push_back(0);
				myMachine.params.push_back(3);
				myMachine.params.push_back(1);
				myMachine.process_event(EvPress());
				myMachine.params.resize(0);
				
			}
		}else if(data.get_d3joints().at(1).x < 0.6&&data.get_d3joints().at(0).x > -0.8){
			myMachine.process_event(EvRelease());
		}
		
		
		
		if(data.get_d3joints().at(1).z < 0.9 && data.get_d3joints().at(1).y > 0){
			int aux; 
			if(!data.get_d3joints().empty()){
			aux =  (int)(data.get_d3joints().at(1).y/step);
			std::cout << aux << "   " << data.get_d3joints().at(1).z << "---" << data.get_d3joints().at(1).y  << std::endl;
			  myMachine.params.push_back(CONTROL);
				myMachine.params.push_back(0);
				myMachine.params.push_back(0);
				myMachine.params.push_back(aux);
			myMachine.process_event(EvSlide());
			myMachine.params.resize(0);
			
			//std::cout << "HERE NO PROBLEMS!!" << std::endl;
		}
			
		}else if(data.get_d3joints().at(1).z > 0.9){
			std::cout << "EVNNNNOOOO SLIDE" << std::endl;
			myMachine.process_event(EvNSlide());
		}	
		
		/*
		if(data.get_h_st().at(0)==2){
			
			
			
			std::cout << "Peta aqui" << std::endl;
			if(!data.get_d3joints().empty()){
			aux = 127 + (int)(data.get_d3joints()[0].x/step);
			std::cout << aux << std::endl;
			midi.setMethod(CONTROL);
			midi.setParams(0,0,aux);
			_midififo.insert(midi);
			isOpen = true;
			std::cout << "HERE NO PROBLEMS!!" << std::endl;
		}	
		}*/
		//std::cout << data.toString() << std::endl;
}

void StateMachine::operator () (){
	myMachine.initiate();
	while(1){
	
	process();
	//std::cout << "I'm in operator while" << std::endl;
	}
}
