#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <boost/mpl/list.hpp>

#include <boost/config.hpp>

#include <ctime>
#include <iostream>
#include <vector>


namespace sc = boost::statechart;
namespace mpl = boost::mpl;

class Active;
class Stopped;

class EvStop : public sc::event< EvStop > {};
class EvStart : public sc::event< EvStart> {};
class EvReset :public sc::event< EvReset > {};
class EvGetMidi : public sc::event< EvGetMidi >
{
    
  public:
   EvGetMidi( std::vector<int> & params );

    void Assign( std::vector<int>  params ) const;
    
  private:
    std::vector<int> & params_;
};



class  PlayPause : public sc::state_machine< PlayPause, Active > {};



class Active : public sc::simple_state< Active, PlayPause, Stopped >
{
  public:
   typedef  sc::transition< EvReset, Active > reactions;
 public:
    Active();  


    std::vector<int> & Params();

    std::vector<int>Params() const;


  private:
   std::vector<int> params_ ;
};

class Running :public sc::simple_state< Running, Active >
{
  public:
    typedef mpl::list<
      sc::custom_reaction< EvGetMidi >,
      sc::transition< EvStop, Stopped >,
      sc::transition<EvStart,Running>
    > reactions;

    Running() ;
    ~Running();

    sc::result react( const EvGetMidi & evt );
   
  private:
    std::vector<int> Params() const;


    std::vector<int> params_;
};

class Stopped : public sc::simple_state< Stopped, Active >
{
	public:
  typedef  mpl::list<
    sc::custom_reaction< EvGetMidi >,
    sc::transition< EvStart, Running >
  > reactions;
  public:
  Stopped()  ;

  sc::result react( const EvGetMidi & evt );

};
