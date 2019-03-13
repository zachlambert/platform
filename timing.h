#ifndef TIMING_H_GUARD
#define TIMING_H_GUARD

class Timer{
public:
	Timer():running(false),timer(0),duration(0){}
	
	void update(float);
	void start(float);
	void stop();
	void reset();
	void fullReset();
	const bool isReady()const;
	
private:

	bool running;
	float timer;
	float duration;
};

#endif