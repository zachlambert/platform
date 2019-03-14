#ifndef PLAYER_H_GUARD
#define PLAYER_H_GUARD

#include "resources.h"
#include "entity.h"
#include "hitbox.h"
#include "motion.h"
#include "animator.h"

class Player: public SpriteEntity{
public:
	Player(const SpriteSheet& spriteSheet, const Resources& resources);

	virtual void update(float seconds);

	Hitbox& h(){ return hitbox; }
	const Hitbox& h()const{ return hitbox; }
	Motion& m(){ return motion; }
	const Motion& m()const{ return motion; }
	Animator& a(){ return animator; }
	const Animator& a()const{ return animator; }

	void jump();
	void startRunning();
	void stopRunning();
	void moveLeft();
	void moveRight();
	void stopHorizontalMovement();

private:

	Hitbox hitbox;
	Motion motion;
	Animator animator;

	const float WALK_SPEED = 600;
	const float RUN_SPEED = 1200;
	const float JUMP_SPEED = 1100;
	const float LARGE_JUMP_SPEED = 1800;

	float horizontalSpeed;

};

#endif