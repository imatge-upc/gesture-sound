/// \file GestoosDetector.cpp

#include <fezoolib/Detection/GestureDetector.hpp>
#include <fezoolib/Core/Visualization.hpp>
#include <fezoolib/Core/ToolConfig.hpp>
#include <fezoolib/Core/Timestamp.hpp>
#include <boost/format.hpp>

cv::Mat heat_prob;

    namespace gesture_tester
    {
        // Tool name
        static const std::string TOOL_NAME   = "Gestoos Detector Demo";
        // Tool brief
        static const std::string BRIEF       = "Run the core detector behind Gestoos on live depth streams\n";
        // Tool description
        static const std::string DESCRIPTION = (std::string) "This demo detects Gestoos in real-time on a RGBD camera at QVGA resolution. \n " +
        "\n" +
        "Current available gestures and class labels:\n"+
	    " 1: Tee\n"+
	    " 2: Pause\n"+
	    " 3: Voldown\n"+
	    " 4: Volup\n"+
	    " 5: Cross\n"+
	    " 6: Ok\n"+
	    " 7: Fwd (note: asymmetric gesture)\n"+
	    " 8: Rewind(note: asymmetric gesture)\n"+
        		"\nFor more details, ask contact@fezoo.cat \n" +
        		"\n\n To run the demo, simply invoke the tool from command line and tell the path to the desired gestures320.ini file" +
        		"\n gestureDemo --ini_file <path to>/gestures320.ini\n\n" +
        "\n\n You can also activate/deactivate gestures in gestures320.ini by removing lines." ;
    }

	using namespace gesture_tester;
	using namespace gestoos;

	class TestConfig : public gestoos::ToolConfig
	{
	public:

		std::string ini_file;
		int score;

		TestConfig(int argc, char* argv[]) :
				gestoos::ToolConfig(TOOL_NAME, BRIEF, DESCRIPTION)
		{

			ini_file ="./../config/gestures320.ini";
			
			#ifdef MSVC
				ini_file ="./../../config/gesturesWin.ini";
			#endif

			score=0;

			//add options
			add_option(ini_file, "ini_file", "Configuration file with gestures and thresholds");
			add_option(score, "score", "Display score map for a given gesture ordinal (the n-th gesture in your gesture.ini)");

			// configure
			configure(argc, argv);

			//read
			read("ini_file", ini_file);
			read("score", score);
		}

	};

int main(int argc, char* argv[])
{

	//Test on a live device
	gestoos::detection::GestureDetector detector;
	int frame_count=0;

	TestConfig cfg(argc, argv);

	detector.init_camera(gestoos::CaptureRGBD::QVGA_30FPS);
	if (!detector.is_connected_to_camera())
	{
		//std::cout << "ERROR:  RGBD sensor not found" << std::endl;
		exit(-1);
	}
	detector.init_detector(cfg.ini_file, ".");
	detector.use_motion_detection(true);


	double minDepth;
	double maxDepth;

	double draw_scale  = 2.0;

#ifndef AVOID_QT
	if (cfg.score) {
		cv::namedWindow("Detection map");
		cv::moveWindow("Detection map", 320*draw_scale, 240*draw_scale);
	}
#endif


	while(1)
	{
		detector.process();
		detector.min_max_depth(minDepth, maxDepth);
		if (minDepth < 600 )
		{
			std::cout << "YOU (OR SOMETHING) ARE TOO CLOSE! " << minDepth << std::endl;
		}

		//Show score map
#ifndef AVOID_QT
		detector.visualize(draw_scale);
		if (cfg.score)
		{
			gestoos::score_heat_map(detector.get_probability_map(cfg.score), heat_prob, 0., 10.);
			cv::resize(heat_prob, heat_prob, cv::Size(0,0), draw_scale, draw_scale);
			cv::imshow("Detection map", heat_prob);
		}
		int id =  detector.get_gesture().id;
		/*if (id > 0 )*/ std::cout << "Detected gesture " << id << std::endl;

		int key = cv::waitKey(1);


		if ( key==27 ) break;
#else
		int id =  detector.get_gesture().id;
		if (id > 0 ) std::cout << "Detected gesture " << id << std::endl;
#endif

	}


	return 0;

}


