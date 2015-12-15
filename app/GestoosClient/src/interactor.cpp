#include "interactor.hpp"

myClientInteractor::myClientInteractor(){
	
	tracker = new Object();
	arr = new Array();
	tracker->set("n_joint", 0);
	tracker->set("static", 0);
	tracker->set("h_st", arr);
	tracker->set("d3joint", arr);
	tracker->set("errors", arr);
	tracker->stringify(std::cout, 0);
	std::cout << std::endl;
	}

bool myClientInteractor::setConection(){
	uri = "http://localhost:9980";
	//uri = "http://localhost:9980";
	std::cout << "Host " + uri.getHost() + " Port " << uri.getPort() << std::endl;
	std::string path(uri.getPathAndQuery());
	if (path.empty()) path = "/";
	session.setHost(uri.getHost());
	session.setPort(uri.getPort());
	
	std::cout << "Host " + session.getHost() + " Port " << session.getPort() << std::endl;
	request.setMethod(HTTPRequest::HTTP_PUT);
	request.setURI(path);
	request.setVersion(HTTPMessage::HTTP_1_1);

	//HTTPRequest request(HTTPRequest::HTTP_PUT, path, HTTPMessage::HTTP_1_1);
	std::cout << "Conection done" << std::endl;
	return true;
}

bool myClientInteractor::sendRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response){
	//tracker->stringify(std::cout, 0);
	try{
	str.clear();
	tracker->stringify(str, 0);
	str >> json;
	//std::cout << "Sending: " << json << std::endl;
	std::cout << response.getStatus() << " " << response.getReason() << std::endl;
	
	if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
	{
		std::ostream& os = session.sendRequest(request);
		//std::cout << "HERE NO PROBLEMS!!" << std::endl;	
		os << json;


		return true;
	}
	else
	{
		//std::cout << "NULL!!!!" << std::endl;
		Poco::NullOutputStream null;
		//StreamCopier::copyStream(rs, null);
		return false;
	}
	
	}catch(Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
}


void myClientInteractor::ToJsonStyle(gestoos::nui::Interactor &_interactor){
	int joints_count = 0,left,right;
	tracker->clear();
	arr->clear();
	left = _interactor.get_hand_gestures().first.id;
	right = _interactor.get_hand_gestures().second.id;
	
	if(left == -1)left = 2;
	if(right == -1)right = 2;
	
		//arr->set(0, _interactor.get_hand_gestures().first.id);
		arr->set(0,left);
		//arr->set(1, _interactor.get_hand_gestures().second.id);
		arr->set(1, right);
	
	
	tracker->set("h_st", *arr);

	arr->clear();
	if(_interactor.get_hands().first.is_present()){
			joints_count=1;
			
					arr->set(0, _interactor.get_hands().first.get_pos().x);
					arr->set(1, _interactor.get_hands().first.get_pos().y);
					arr->set(2, _interactor.get_hands().first.get_pos().z);
		//std::cout << _interactor.get_hands().first.get_pos().x << std::endl;
	}
	if(_interactor.get_hands().second.is_present()){
		joints_count = 2; 
		
					arr->set(3, _interactor.get_hands().second.get_pos().x);
					arr->set(4, _interactor.get_hands().second.get_pos().y);
					arr->set(5, _interactor.get_hands().second.get_pos().z);
		
	}


	tracker->set("d3joint", *arr);
	arr->clear();
	tracker->set("n_joint", 2);
	tracker->set("static", _interactor.get_gesture().id);
	tracker->set("errors", *arr);

	

	

	
	tracker->stringify(std::cout, 0);
	std::cout << "\n\n";
	
	
	
}
void myClientInteractor::myProces(gestoos::nui::Interactor &_interactor){
	
	ToJsonStyle(_interactor);
	
	std::cout << "\n\n";
    std::cout << "Host " + session.getHost() + " Port " << session.getPort() << std::endl;
	sendRequest(session, request, response);
	
	}

int main(int argc, char* argv[]){
	myClientInteractor client;
	gestoos::nui::Interactor interactor;
	  interactor.init("./../config/whai.ini", "./../config/gestures320.ini", "./../config/hand.ini");
	  interactor.set_draw_window(true);
	  client.setConection();
	  
	  
	  while(1){
		  interactor.process();
		 client.myProces(interactor);
		 
	  }
	return 0;
	
	
	
}
