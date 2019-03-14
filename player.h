#ifndef PLAYER_H_GUARD
#define PLAYER_H_GUARD

#include "resources.h"
#include "entity.h"
#include "hitbox.h"
#include "motion.h"
#include "animator.h"

class Player: public SpriteEntity, public Physical, public Animated{
public:
	Player(const SpriteSheet& spriteSheet, const Resources& resources);

	virtual sf::Transformable& t(){ return SpriteEntity::t(); }

	virtual void update(float seconds);

	void moveLeft();
	void moveRight();
	void stopHorizontalMovement();

	void jump();
	void setOffGround();
	void setOnGround();

	void startRunning();
	void stopRunning();

private:

	const float WALK_SPEED = 600;
	const float RUN_SPEED = 1200;
	const float JUMP_SPEED = 1100;
	const float LARGE_JUMP_SPEED = 1800;

	float horizontalSpeed;

};

#endif