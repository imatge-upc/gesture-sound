
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>
#include <boost/config.hpp>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

class Sliding: public  sc::simple_state<Sliding, Control>
{
	public: 
	typedef mpl::list<
	sc::transition<EvNSlide,Iddle>,
	sc::custom_reaction<EvSlide>
	> reactions;

	public:
	Sliding();
	~Sliding();
	sc::result react(const EvSlide &);
};

