#include "utility.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

int stringToInt(const std::string& string){
	std::stringstream parser(string);
	int x=0;
	try{
		parser>>x;
	}catch(std::exception e){}
	return x;
}

float stringToFloat(const std::string& string){
	std::stringstream parser(string);
	float x=0.0;
	try{
		parser>>x;
	}catch(std::exception e){}
	return x;
}
