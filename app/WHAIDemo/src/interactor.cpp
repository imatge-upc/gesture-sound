#include <fezoolib/Tracking/WHAITracker.hpp>
#include <fezoolib/NUI/Interactor.hpp>
#include <fezoolib/Core/ToolConfig.hpp>
#include <fezoolib/Core/Timestamp.hpp>
#include <fezoolib/Core/DepthGeometry.hpp>
#include <boost/thread/thread.hpp>

#include <signal.h>
#include <cstdio>
#include <cstdlib>

#define POINTS 3

static bool exit_program;

namespace WHAIConf
{
// Tool name
static const std::string TOOL_NAME   = "WHAI Tracker Demo";
// Tool brief
static const std::string BRIEF       = "Track hands easily with WHAI Tracker.\n";
// Tool description
static const std::string DESCRIPTION = (std::string) "The tracker will run in real-time from an OpenNI compatible camera.\n"+
		"If a test_sequence (.oni) is provided, it will run offline.\n" +
		"Config file is in whai.ini. \n"+
		"This file is important, because it sets the threshold in order to init tracks\n" +

		"\nFor more details, ask contact@fezoo.cat \n" +

		"\nHave a WHAI tracking!\n\n"+
		"\n                        The FEZOO LABS team.\n"+
		"\n==============================================================================\n";
}

using namespace WHAIConf;

class TestConfig : public gestoos::ToolConfig
{
public:
	std::string test_sequence;
	std::string ini_file;
	bool gpu;


	TestConfig(int argc, char* argv[]) :
		gestoos::ToolConfig(TOOL_NAME, BRIEF, DESCRIPTION)
	{
		test_sequence="";
		ini_file = "./../config/whai.ini";
		gpu = false;
#ifdef MSVC
		ini_file = "./../config/whaiWin.ini";
#endif
		//add options
		add_option(test_sequence, "test_sequence", "Full path of the test sequence, in oni format. If not provided, hand tracker runs on the camera.");
		add_option(ini_file, "ini_file", "Configuration file for WHAI Tracker (whai.ini).");
		add_flag(gpu, "gpu", "use gpu computation ", 'g');

		// configure
		configure(argc, argv);

		//read
		read("test_sequence", test_sequence);
		read("ini_file", ini_file);
		read("gpu", gpu);

	}

};
gestoos::CaptureRGBD capture;
int main(int argc, char* argv[])
{
	gestoos::nui::Interactor interactor;
	interactor.init("./../config/whai.ini", "./../config/gestures320.ini", "./../config/hand.ini");//PROVAR AMB DIFERENTS INI COM DIU API...
	
	interactor.set_draw_window(true);
	//interactor.get_hands().first.set_mode(gestoos::nui::Hand::MODE_DTW);
	double draw_scale  = 2.0;
	cv::Mat depth_map;
	cv::Mat color_img;
	
	

//cv::namedWindow("Detection map");
	//	cv::moveWindow("Detection map", 320*draw_scale, 240*draw_scale);
	
	
	while(1){
		
	
		interactor.process();
	/*	if(interactor.get_hands().first.is_present()){
		std::cout << "Getting hands..." << std::endl;
		std::pair< gestoos::nui::Hand, gestoos::nui::Hand > hands = interactor.get_hands();
		std::cout << "Getting first..." << std::endl;
		gestoos::nui::Hand hand = hands.first;
		std::cout << "Getting pos..." << std::endl;
		cv::Point3f pp = hand.get_world_pos();
		std::cout << "Getting X..." << std::endl;
		float xx = pp.x;
		std::cout <<"X = " << xx << std::endl;
}*/
		if(interactor.get_hands().first.is_present()){
			
			std::cout << "FIRST" << std::endl;
			//std::cout << "Peta aquí" << std::endl;
			
			//float x = ((interactor.get_hands().first.get_pos().x*2)-320)*(interactor.get_hands().first.get_pos().z/534)/1000;
			//float y = ((interactor.get_hands().first.get_pos().y*2)-240)*(interactor.get_hands().first.get_pos().z/535)/1000;
				//std::cout <<"X = " << x << "Y = " << y <<"Z = "<<interactor.get_hands().first.get_pos().z/1000 <<std::endl;
				//std::cout <<"Xpx = " << interactor.get_hands().first.get_pos().x << "Z = "<<interactor.get_hands().first.get_pos().z << std::endl;
				std::cout <<"X = "<< interactor.get_hands().first.get_world_pos().x<<std::endl;
				//std::cout <<"Y = "<< interactor.get_hands().first.get_pos().y<<std::endl;
				//std::cout <<"Z = "<< interactor.get_hands().first.get_pos().z<<std::endl;
		
	}
	if(interactor.get_hands().second.is_present()){
		std::cout << "SECOND" << std::endl;
				std::cout <<"X = " <<interactor.get_hands().second.get_pos().x<< std::endl;
				//std::cout <<"Y = "<<interactor.get_hands().second.get_pos().y<<std::endl;
				//std::cout <<"Z = "<<interactor.get_hands().second.get_pos().z<<std::endl;
	}
	}
	
	return 0;
}




