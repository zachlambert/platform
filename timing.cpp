#include "timing.h"

void Timer::update(float seconds){
	if(running){
		timer+=seconds;
	}
}
void Timer::start(float duration){
	running=true;
	this->duration=duration;
}
void Timer::stop(){
	running=false;
	timer=0;
	duration=0;
}
void Timer::reset(){
	timer-=duration;
}
void Timer::fullReset(){
	timer=0;
}
const bool Timer::isReady()const{
	return timer>=duration;
}