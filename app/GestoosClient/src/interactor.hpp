#include <iostream>
#include <fezoolib/Tracking/WHAITracker.hpp>
#include <fezoolib/NUI/Interactor.hpp>
#include <fezoolib/Core/ToolConfig.hpp>
#include <fezoolib/Core/Timestamp.hpp>
#include <fezoolib/Core/DepthGeometry.hpp>

#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

using namespace Poco::JSON;

class myClientInteractor{
	
	private: 
	//JSON atributes
	Object::Ptr tracker;
	Array::Ptr arr;
	std::string json;
	std::stringstream str;
	


	//Network Atributes

	HTTPClientSession session;
	HTTPRequest request;
	HTTPResponse response;
	URI uri;
	
	
	public: 
	
	myClientInteractor();
	
	bool setConection();
	
	bool sendRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response);
	
	void ToJsonStyle(gestoos::nui::Interactor &_interactor);
	
	void myProces(gestoos::nui::Interactor &_interactor);
	
	
	
	
	
};
