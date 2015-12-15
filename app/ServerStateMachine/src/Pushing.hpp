
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>
#include <boost/config.hpp>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

class Pushing: public sc::simple_state<Pushing,Control>
{
		public: 
	
		
		typedef mpl::list<
      
      sc::transition< EvRelease, Iddle >
    > reactions;
		
		public:
		Pushing();
		~Pushing();
		/*
		sc::result react( const EvRelease & );
		sc::result react(const EvPress &);*/
};
