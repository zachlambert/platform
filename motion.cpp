#include "motion.h"

void Motion::setInFreeFall(bool inFreeFall){
	this->inFreeFall = inFreeFall;
	if(inFreeFall){
		acceleration.y = 1960;
	}else{
		acceleration.y = 0;
	}
}