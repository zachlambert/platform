#ifndef PARTICLE_SYSTEM_H_GUARD
#define PARTICLE_SYSTEM_H_GUARD

#include <vector>
#include <SFML/Graphics.hpp>
#include "entity.h"
#include "vertexArraySprite.h"

//Don't change the position of this class, keep at 0,0.
//The position of the particles is relative to 0,0.
//To change the position of the source, use setSource.
//This means that you can have a moving source, independent
//of the particles.
//Also, don't rotate or scale.

//Particles are recycled.

//This class is an example of a particle system.
//It makes sense to make different particle effects different classes, not derived from a base particle system class.

class ParticleSystem: public EntityTemplate<Entity,VertexArraySprite>{
public:
	ParticleSystem(int);
	
	void update(float);
	
	void setSource(float x,float y){ source=sf::Vector2f(x,y); }
	void setSource(const sf::Vector2f& source){ this->source=source; }
	
private:
	struct Particle{
		sf::Vector2f velocity;
		float lifetime;
		Particle():velocity(0,0),lifetime(0){}
	};
	
	void resetParticle(std::size_t);
	
	std::vector<Particle> particles;
	sf::Vector2f source;
};

#endif