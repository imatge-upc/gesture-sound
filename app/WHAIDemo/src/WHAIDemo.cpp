/// \file WHAIDemo.cpp

#include <fezoolib/Tracking/WHAITracker.hpp>
#include <fezoolib/Core/ToolConfig.hpp>
#include <fezoolib/Core/Timestamp.hpp>
#include <fezoolib/Core/DepthGeometry.hpp>
#include <boost/thread/thread.hpp>

#include <signal.h>

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

//Test on a live device
gestoos::CaptureRGBD capture;

#ifndef MSVC
struct sigaction sa;
#endif

static void got_signal(int)
{

	std::cout<<"\nstopping camera\n"<<std::flush;
	capture.stop();

	std::cout<<"exit(0)\n"<<std::flush;
	exit(0);
}

/// Renderinf function called by the render thread
void render_func(const cv::Mat & depth_map, gestoos::tracking::WHAITracker * tracker)
{
	std::vector< gestoos::tracking::ObjectTrack *> hands;
	int i = 0, value;
	bool are_two = false;
	//gestoos::tracking::ObjectTrack::point_type position;
	hands = tracker->active_tracks();
	
	
			if(!hands.empty()){
		    for(i = 0; i<hands.size(); ++i){
				    //position = hands[i]->get_position(); 
					std::cout << "Hand " << i << ": X =" << hands[i]->get_pt_world ().x << std::endl;
					//~ "  Y =" << hands[i]->get_position().y << "  Z(mm) =" << hands[i]->get_position().z << 
					
				
			}
			}
#ifndef AVOID_QT
	//Exit if empty image!
	if( depth_map.cols == 0 )
	{
		return;
	}

	cv::Mat color_img;

#ifndef NDEBUG
	std::cout << "DBG: Depth map conversion \n" << std::flush;
#endif

	// Normalizing depth data values
	depth_map.convertTo(color_img, CV_8UC1, 1/15.0);
	// Converting from Grayscale to RGB
    cv::cvtColor(color_img, color_img, CV_GRAY2BGR);

    //Set to false to hide trajectory
	bool show_trajectory = true;
	//Comment to hide hand position and trajectory
	tracker->visualize(color_img, 3, show_trajectory);
	//Comment to hide hand labels
	tracker->show_labels(color_img, 3);
	
#ifndef NDEBUG
	std::cout << "DBG: Drawing... \n" << std::flush;
	

#endif
	cv::imshow("Tracker Output", color_img);
	
#ifndef NDEBUG
	std::cout << "DBG: Done \n" << std::flush;
#endif

	// pthreads and imshow don't get along in Apple machines
#ifndef __APPLE__
#ifndef MSVC

	int key = cv::waitKey(1);
	if (key==27)
	{
		exit_program = true;
		// break;
	}
#endif
#endif

#endif //AVOID_QT



}


int main(int argc, char* argv[])
{
	signal(SIGINT, got_signal);

	exit_program = false;

	TestConfig cfg(argc, argv);

	//Declare WHAITracker
	gestoos::tracking::WHAITracker whai;

	//To store the current frame num
	gestoos::tracking::ObjectTrack::ts_type frame=0;

	if (cfg.test_sequence!="")
	{
		capture.init(cfg.test_sequence);
		std::cout << "NOOOOOOO" << std::endl;
	}
	else
	{
		bool camera_ready=capture.init("",  0,  gestoos::CaptureRGBD::QVGA_30FPS); //, true, false);
		std::cout << "YEEEEEEEEES" << std::endl;
		if (!camera_ready)
		{
			exit(-1);
		}
	}

	if(cfg.gpu)
	{
		std::cout << "using gpu" << std::endl;
	}

	//Configure the tracker
	whai.init(cfg.ini_file
#ifdef USE_GLSL
			, cfg.gpu
#endif
	);




	/*
	 * Tracking inputs
	 */
	double draw_scale  = 2.0;
	
	cv::Mat depth_map;
	cv::Mat color_img;
#ifndef AVOID_QT
	cv::namedWindow("Tracker Output"); //avans ("Tracker Output", 1);
	cv::moveWindow("Tracker Output", 320*draw_scale, 240*draw_scale);
#endif
	while(1)
	{
		gestoos::Timestamp ts = gestoos::clock_ts();

		/*
		 * Capture
		 */
		capture.get_depth_frame();
		depth_map = capture.depth_frame();


		if (depth_map.empty())
		{
			std::cout << "Error: RGBD Sensor not found or disconnected " << std::endl;
			exit(-1);
		}
		/*
		 * Filter data, mainly filling depth holes
		 */
		depth_map=gestoos::depth_error_filter(depth_map);

		/*
		 * Detect hands using WHAI
		 */
		whai.update(depth_map, frame);

		//Launch render thread for 1 frame
		//Note: This works flawlessly on Ubuntu, but not on OSX:
		//http://answers.opencv.org/question/30507/imshow-not-working-in-pthread-macosx-109/
#ifndef AVOID_QT
#ifndef MSVC		
		//boost::thread render_thread(render_func, depth_map, &whai);
		render_func(depth_map,  &whai);
		                int key = cv::waitKey(1);
		                if (key==27)
		                {
		                        exit_program = true;
		                        // break;
		                }
#endif

#ifdef __APPLE__
		int key = cv::waitKey(1);
		if (key==27)
		{
			exit_program = true;
			// break;
		}
#endif

#ifdef MSVC
		render_func(depth_map,  &whai);
		int key = cv::waitKey(1);
		if (key==27)
		{
			exit_program = true;
			// break;
		}
#endif
#else //AVOID_QT
		std::vector<gestoos::tracking::ObjectTrack*> active_tracks = whai.active_tracks();
		std::vector<gestoos::tracking::ObjectTrack*>::const_iterator iter;
		for (iter = active_tracks.begin(); iter!=active_tracks.end(); ++iter)
		{
			std::cout << "Hand " << (*iter)->get_id() << " " << (*iter)->get_position_2d() << std::endl;
		}
#endif //AVOID_QT


		if(exit_program) break;
		++frame;


		//gestoos::Timestamp ts2 = gestoos::clock_ts();

		//gestoos::Timestamp diff_ts=ts2-ts;


		//double elapsed= diff_ts.seconds()*1000.0 + diff_ts.millis();


		//std::cout << "\r" << elapsed << " ms | "<< 1/(elapsed/1000.0) << " fps" <<  std::flush;


	}

	capture.stop();
	return 0;
}




