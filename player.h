#ifndef PLAYER_H_GUARD
#define PLAYER_H_GUARD

#include "entity.h"
#include "hitbox.h"
#include "motion.h"
#include "animator.h"

class Player: public SpriteEntity{
public:
	Player(const SpriteSheet& spriteSheet) :SpriteEntity(spriteSheet), hitbox(), motion(), animator() {}

	virtual void update(float seconds);

	Hitbox& h(){ return hitbox; }
	const Hitbox& h()const{ return hitbox; }
	Motion& m(){ return motion; }
	const Motion& m()const{ return motion; }
	Animator& a(){ return animator; }
	const Animator& a()const{ return animator; }

private:

	Hitbox hitbox;
	Motion motion;
	Animator animator;
};

#endif