#include "d3message.hpp"

d3data::d3data(){}

void d3data::put_n_joints(int _n_joints){
	
	n_joints = _n_joints;
}

void d3data::put_h_st(int _h_st){
	h_st.push_back(_h_st);
}

void d3data::put_gstatic(int _gstatic){

	gstatic = _gstatic;
}

void d3data::put_d3joints(cv::Point3f _d3joints){

	d3joints.push_back(_d3joints);
}

void d3data::put_errors(std::string _errors){

	errors.push_back(_errors);
}

int d3data::get_n_joints(){
	return n_joints;
}
	
int d3data::get_gstatic(){
	return gstatic;
}

std::vector<int> d3data::get_h_st(){
	return h_st;
}

std::vector<cv::Point3f> d3data::get_d3joints(){
	return d3joints;
}

std::vector<std::string> d3data::get_errors(){
	return errors;
	}

std::string d3data::toString(){
	std::stringstream ret ; 
	std::string res;
	ret << "N_joints: " << n_joints << " static: " << gstatic << " H_st: [";
	
	for(int i = 0; i < h_st.size(); ++i){
		ret << h_st.at(i) << ",";
	} 
	
	ret << "] d3joints: ["; 
	for(int i = 0; i <d3joints.size(); ++i ){
		ret << d3joints.at(i) << ",";
		
	}
   
   ret << "]";
    res = ret.str();
   return res;
	
	
}


d3message::d3message(){
	names.push_back("n_joint");
	names.push_back("h_st");
	names.push_back("static");
	names.push_back("d3joint");
	names.push_back("errors");
	}
/*
 * Function that save to a variable called data of type d3data to save de JSON values 
 * with the correct type, and then could work with them
 * 
 * */
bool d3message::strToObject(std::string str){
	
	jsonParser.parse(str);
	Poco::Dynamic::Var loParsedJsonResult = jsonParser.result();
	Poco::Dynamic::Var  auxiliar;
	
	jsObject = loParsedJsonResult.extract<Object::Ptr>();
	
	for (std::vector<std::string>::iterator it = names.begin() ; it != names.end(); ++it){

		auxiliar = jsObject->get(*it);
		if (auxiliar.isEmpty()){

			std::cout << *it << "Is empty" << std::endl;
			return false; //Control if one of the params is empty 

		}
		
	}

	//NJOINTS
	data.put_n_joints(jsObject->get(names[0]));

	//HAND STATE
	data.put_h_st(jsObject->getArray(names[1])->get(0));
	data.put_h_st(jsObject->getArray(names[1])->get(1));

	//STATIC 
	data.put_gstatic(jsObject->get(names[2]));
	cv::Point3f _point3d;
	//D3POINTS
	int yy = 0;
	for (int ii = 0; ii < jsObject->getArray(names[3])->size()/3; ++ii){
		
		_point3d.x = jsObject->getArray(names[3])->get(yy);
		_point3d.y = jsObject->getArray(names[3])->get(yy+1);
		_point3d.z = jsObject->getArray(names[3])->get(yy+2);
			yy+=3;
		data.put_d3joints(_point3d);
	
	}
	
	//ERRORS
	for (int i = 0; i < jsObject->getArray(names[4])->size(); ++i){

		data.put_errors(jsObject->getArray(names[4])->get(i));

	}


	 jsObject->stringify(std::cout,0);
	
	return true;
}

d3data d3message::getd3data(){
	std::cout << "Returning data!!" << std::endl;
	return data;
}
	
/*
int main (int argc, char* argv[]){
	
	d3message d3; 
	std::string str = "{\"d3joint\":[-0.64118207,-0.33531687,1.2718775,0.024604935,-0.2777965,1.3148462],\"errors\":[],\"h_st\":[1,3],\"n_joint\":2,\"static\":-1}";
	d3.strToObject(str);
	std::cout << d3.getd3data().toString() << std::endl;
	
	return 0;
	
	
}
*/



