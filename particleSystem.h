#ifndef PARTICLE_SYSTEM_H_GUARD
#define PARTICLE_SYSTEM_H_GUARD

#include "entity.h"
#include "particleSystemSprite.h"

class ParticleSystem : public EntityTemplate<Entity, ParticleSystemSprite> {
public:
	ParticleSystem(int count):EntityTemplate(ParticleSystemSprite(count)) {}

	virtual void update(float seconds) { s().update(seconds); }

};

#endif