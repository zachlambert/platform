#include "entity.h"

void entityMove(Entity& entity, float seconds){
	//Ensure that entity.m()->update(seconds) has been called before this function

	if(entity.t() && entity.m()){
		entity.t()->move(entity.m()->getDisplacement());
	}
}

void entityMoveBack(Entity& entity, float seconds){
	if (entity.t() && entity.m()) {
		entity.t()->setPosition(entity.t()->getPosition() - entity.m()->getDisplacement());
	}
}

void entityUpdateAnimation(Entity& entity, float seconds){
	if(entity.a()){
		entity.a()->update(seconds);
		if(!entity.a()->isPaused()){
			entity.setFrame(entity.a()->getFrame());
		}
	}
}