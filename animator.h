#ifndef ANIMATOR_H_GUARD
#define ANIMATOR_H_GUARD

#include <string>
#include "resources.h"
#include "timing.h"

class Animator{
public:
	Animator(const Resources& resources):resources(resources),animationId(-1),animation(0),frameIndex(0),timer(),speed(1),looping(false),paused(false){}
	const bool update(float);
	const int getFrame()const{ return frameIndex; }
	
	void startAnimation(resource::Animation animation,bool);
	void pauseAnimation();
	void resumeAnimation();
	void resetAnimation();
	bool isInAnimation(resource::Animation animation);
	bool isPaused()const{ return paused; }
	void setAnimationSpeed(float speed){ this->speed = speed; }
	const float getAnimationSpeed()const{ return speed; }
private:
	const Resources& resources;
	int animationId;
	const Animation* animation;
	int frameIndex;
	Timer timer;
	float speed;
	bool looping;
	bool paused;
};

#endif