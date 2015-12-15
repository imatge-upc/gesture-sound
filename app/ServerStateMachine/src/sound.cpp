#include "sound.hpp"

#define snd_seq_flush_output(x) snd_seq_drain_output(x)
Sound::Sound(FIFOmidi& fifo):_midififo(fifo){
}

void Sound::Open(){
	
	if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_OUTPUT, 0) < 0) {
    fprintf(stderr, "Error opening ALSA sequencer.\n");
    exit(1);
	}
	snd_seq_set_client_name(seq_handle, "ALSA Sequencer Demo");
	if ((my_port = snd_seq_create_simple_port(seq_handle, "ALSA Sequencer Demo",
			SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,   
            SND_SEQ_PORT_TYPE_MIDI_GENERIC |
            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "Error creating sequencer port.\n");
    exit(1);
}
}
	
void Sound::send_event(int do_flush){
	
	snd_seq_ev_set_direct(&ev);
    snd_seq_ev_set_source(&ev, my_port);
    snd_seq_ev_set_dest(&ev, seq_client, seq_port);

    snd_seq_event_output(seq_handle, &ev);
    if (do_flush)
    snd_seq_flush_output(seq_handle);
}

void Sound::note_on(int chan_no ,int note, int vel){
	snd_seq_ev_set_noteon(&ev, chan_no, note, vel);
    //Sound::send_event(1);
}

	void Sound::control(int chan_no, int type, int val){
		snd_seq_ev_set_controller(&ev,chan_no,type, val);
		//send_event(1);
	}
	
void Sound::setEvent(midiData event){
	int chanel, note, val; 
	
	
		
		
	
	switch(event.getMethod()){
		
		case NOTEON: 
					note_on(event.getParams()[0], event.getParams()[1], event.getParams()[2]);
					break;
		case CONTROL:
					std::cout << "CONTROOOOOOL!!"<< std::endl;
					control(event.getParams()[0], event.getParams()[1], event.getParams()[2]);
					break;
		
	}
	send_event(1);
}

void Sound::operator ()(){
	
	Open();
	
	while(1){
		setEvent(_midififo.get());
		
		
	}
	
	
}



