#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/StreamCopier.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "StateMachine.hpp"

#include <iostream>


using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::StreamCopier;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


class TrackerRequestHandler: public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	TrackerRequestHandler(FIFOd3data& datafifo): _datafifo(datafifo)
	{
	}
	
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		
		std::string str;
		Application& app = Application::instance();
		request.stream() >> str;
		//app.logger().information("I'm here\n\n\n");
		//app.logger().information("Request from " + request.getURI() + " Type: " + request.getMethod()+ "Stream " + str);
		if(parser.strToObject(str)){
			
			_datafifo.insert(parser.getd3data());
			//app.logger().information(_datafifo.get().toString());
		}
			
	
		response.send();
		//std::cout << "HERE NO PROBLEMS!!" << std::endl;	
		//done = true;
	
		//std::cout << "I'm here ???" << std::endl;

	
		
		//StreamCopier::copyStream(request.stream(), std::cout);
		
		

		
	}
	
private:
FIFOd3data& _datafifo;
d3message parser;
bool done = false;
};


class TrackerRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	TrackerRequestHandlerFactory(FIFOd3data& datafifo):_datafifo(datafifo)
		
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getMethod() == "PUT"){
			
			return new TrackerRequestHandler(_datafifo);
		}
		else
			return 0;
	}
	
private:
	std::string _format;
	FIFOd3data& _datafifo;
};


class HTTPTimeServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the HTTPTimeServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (HTTPTimeServer.properties),
	/// copy the file to the directory where the HTTPTimeServer executable
	/// resides. If you start the debug version of the HTTPTimeServer
	/// (HTTPTimeServerd[.exe]), you must also create a copy of the configuration
	/// file named HTTPTimeServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9980) and the format of the date/time string sent back to the client.
	///
	/// To test the TimeServer you can use any web browser (http://localhost:9980/).
{
public:
	HTTPTimeServer(FIFOd3data& fifo): myFIFO(fifo)
	{
	}
	
	~HTTPTimeServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
		
	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A web server that serves the current date and time.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short port = (unsigned short) config().getInt("HTTPTimeServer.port", 9980);
			std::string format(config().getString("HTTPTimeServer.format", DateTimeFormat::SORTABLE_FORMAT));
			int maxQueued  = config().getInt("HTTPTimeServer.maxQueued", 100);
			int maxThreads = config().getInt("HTTPTimeServer.maxThreads", 16);
			ThreadPool::defaultPool().addCapacity(maxThreads);
			
			HTTPServerParams* pParams = new HTTPServerParams;
			pParams->setMaxQueued(maxQueued);
			pParams->setMaxThreads(maxThreads);
			
			// set-up a server socket
			ServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new TrackerRequestHandlerFactory(myFIFO), svs, pParams);
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}
	
private:
	bool _helpRequested = false;
	FIFOd3data& myFIFO;
};


int main(int argc, char** argv)
{
	FIFOd3data myFIFO;
	FIFOmidi midiFIFO;
	Sound sound(midiFIFO);
	HTTPTimeServer app(myFIFO);
	StateMachine stm(myFIFO, midiFIFO);
	boost::thread stm_th(boost::ref(stm));
	boost::thread sound_th(boost::ref(sound));
	
	return app.run(argc, argv);
}
