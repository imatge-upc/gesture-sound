#include "FIFOmididata.hpp"
#include <alsa/asoundlib.h>

class Sound {
	
	private:
	FIFOmidi& _midififo; 
	snd_seq_t *seq_handle = NULL;
	int  my_port;
	int seq_client = SND_SEQ_ADDRESS_SUBSCRIBERS, seq_port = 0;
	int chan_no;
	snd_seq_event_t ev;
	
	
	public:
	
   Sound(FIFOmidi& fifo);
	 //snd_seq_t* open_seq();
   // void send_event(int do_flush);
    //void note_on(int chan_no ,int note, int vel);
    void Open();
	
	void send_event(int do_flush);
    void note_on(int chan_no ,int note, int vel);
	void control(int chan_no, int type, int val);
	
	void setEvent(midiData event);
	
	void operator ()();


	
	
};

