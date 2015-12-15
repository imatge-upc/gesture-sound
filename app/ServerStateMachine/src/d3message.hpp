#include <iostream>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include "opencv2/core.hpp"
using namespace Poco::JSON;
class d3data{
	
	private: 
	int n_joints, gstatic;
	std::vector<int> h_st;
	std::vector<cv::Point3f> d3joints;
	std::vector<std::string> errors;
	
	public: 
	
	d3data();
	
	void put_n_joints(int _n_joints);
	void put_h_st(int _h_st);
	void put_gstatic(int _gstatic);
	void put_d3joints(cv::Point3f _d3joints);
	void put_errors(std::string _errors );
	
	int get_n_joints();
	int get_gstatic();
	std::vector<int> get_h_st();
	std::vector<cv::Point3f> get_d3joints();
	std::vector<std::string> get_errors();
	
	std::string toString();
};

class d3message {
	
	private:
	
	
	
	std::vector<std::string> names;
	
	
	std::string strjson;
	Parser jsonParser;
	Object::Ptr jsObject = new Object();
	Array::Ptr arr = new Array();

	d3data data;
	
	public: 
	
	d3message();
	

	
	bool strToObject(std::string str);
	d3data getd3data();
	
	
	
};
