
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>
#include <boost/config.hpp>
#include "sound.hpp"

namespace sc = boost::statechart;
namespace mpl = boost::mpl;


class Control;
class Pushing;
class Sliding;
class Iddle;


class EvSlide: public sc::event<EvSlide>{};
class EvNSlide: public sc::event<EvNSlide>{};
class EvPress: public sc::event<EvPress>{};
class EvRelease: public sc::event<EvRelease>{};

class Machine: public sc::state_machine<Machine, Control>
{
		public: 
		bool ispause, issent;
		FIFOmidi& _midififo;
		midiData midi;
		std::vector<int> params; //0 Method 123 midi mssg
		
		public:
		//Machine();
		Machine(FIFOmidi& m_fifo);
		//bool isPushed(){return ispushed;};
		//bool isSend(){return issent;};
		//void pushed(bool push){ispushed = push;};
		//void sent(bool send_){issent = send_;};
		
		
};

class Control: public sc::simple_state<Control, Machine, Iddle>
{
	public:
		Control();
		~Control();
		
};

class Iddle: public sc::simple_state<Iddle, Control>
{
	public:
		Iddle();
		~Iddle();
		sc::result react (const EvPress &);
		sc::result react (const EvSlide &);
	public: 
	typedef mpl::list<
	 sc::custom_reaction<EvPress> ,
	 sc::custom_reaction<EvSlide> > reactions;
};
