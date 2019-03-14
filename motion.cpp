#include "motion.h"

void Motion::update(float seconds){
	//Use semi-implicit euler
	displacement = velocity * seconds;
	velocity += acceleration * seconds;
}

void Motion::setInFreeFall(bool inFreeFall){
	this->inFreeFall = inFreeFall;
	if(inFreeFall){
		acceleration.y = 1960;
	}else{
		acceleration.y = 0;
	}
}