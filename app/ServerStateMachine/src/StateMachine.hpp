#include "FIFOd3data.hpp"
//#include "sound.hpp"
//#include "PlayPause.hpp"
#include "Control.hpp"

class StateMachine{
	
	private: 
	
	FIFOd3data& _sharedfifo;
	
	bool ispause1 = true, ispause2 = true;
	Machine myMachine;
	//PlayPause playpause;
	
	public: 
	
	float last_x = 0, last_y = 0, last_z = 0; //NEW!!!!!
	
	
	public: 
	
	StateMachine(FIFOd3data& d_fifo, FIFOmidi& m_fifo);
	
	void set_parameters(int method, int control, int note, int val );
	
	void process ();
	
	void operator ()();
	
	
};
