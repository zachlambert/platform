#include "animator.h"

const bool Animator::update(float seconds){
	bool updated=false;
	
	if(animation!=0 && !paused){
		if(timer.isReady()){
			frameIndex++;
			if(frameIndex>=animation->frames.size()){
				frameIndex=0;
				if(!looping){
					timer.stop();
					animation=0;
					return false;
				}
			}
			updated = true;
			timer.reset();
		}
		timer.update(seconds*speed);
	}
	
	return updated;
}

void Animator::startAnimation(resource::Animation animationId,bool looping){
	this->animationId = static_cast<int>(animationId);
	this->animation = &(resources.getAnimation(animationId));
	frameIndex=0;
	this->looping = looping;
	timer.start(animation->duration/animation->frames.size());
}
void Animator::pauseAnimation(){
	paused=true;
}
void Animator::resumeAnimation(){
	paused=false;
}
void Animator::resetAnimation(){
	frameIndex=0;
	timer.fullReset();
}
bool Animator::isInAnimation(resource::Animation animationId){
	return (this->animationId == static_cast<int>(animationId));
}