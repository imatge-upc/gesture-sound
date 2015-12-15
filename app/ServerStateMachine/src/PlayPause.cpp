//////////////////////////////////////////////////////////////////////////////
// Copyright 2002-2006 Andreas Huber Doenni
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
// The following code implements the state-machine (this version details an
// alternative way of retrieving the elapsed time from the main program):
//
//  --------------------------------
// |                                |
// |           O     Active         |
// |           |                    |<----
// |           v                    |     | EvReset
// |  ----------------------------  |     |
// | |                            | |-----
// | |         Stopped            | |
// |  ----------------------------  |
// |  |              ^              |
// |  | EvStartStop  | EvStartStop  |<-----O
// |  v              |              |
// |  ----------------------------  |
// | |                            | |
// | |         Running            | |
// |  ----------------------------  |
//  --------------------------------


#include "PlayPause.hpp"

namespace sc = boost::statechart;
namespace mpl = boost::mpl;


   EvGetMidi::EvGetMidi( std::vector<int> & params ) : params_( params ) {}

    void EvGetMidi::Assign( std::vector<int>  params ) const
    {
		//std::cout << "Aqui!!"<< std::endl;
      params_ = params;
	}




    Active::Active()  
    {
		std::cout << "Active"<< std::endl;
		params_.resize(0);
		}

    std::vector<int> & Active::Params()
    {
      return params_;
    }

    std::vector<int> Active::Params() const
    {
      return params_;
    }




    Running::Running()  {
		std::cout << "Running " << std::endl;
		}

    Running::~Running()
    {
     // context< Active >().Params() = Params();
    }

    sc::result Running::react( const EvGetMidi & evt )
    {
		
      evt.Assign( Params() );
      return discard_event();
    }


    std::vector<int> Running::Params() const
    {
		std::cout << "Paramsscons Running!!"<< std::endl;
	
	std::vector<int> vector(0);
	vector.push_back(0);
	vector.push_back(0);
	vector.push_back(1);

      return vector;
    }



  
  Stopped::Stopped()  {
		std::cout << "Stopping " << std::endl;
		}

  sc::result Stopped::react( const EvGetMidi & evt )
  {
	  std::cout << "React Stopped!!"<< std::endl;
	  //std::cout << "here!"<< std::endl;
	  std::vector<int> vector(0);
    evt.Assign( vector);
    return discard_event();
  }
/*

int main(int argc, char* argv[]){
	PlayPause playpause;
	std::vector<int> ref;
	playpause.initiate();
	std::cout << "Event"<< std::endl;
	playpause.process_event(EvGetMidi(ref));
	playpause.process_event(EvStop());
	playpause.process_event(EvStart());
	playpause.process_event(EvGetMidi(ref));

	std::cout << "Event"<< std::endl;
	playpause.process_event(EvStop());
	playpause.process_event(EvGetMidi(ref));

	}*/
