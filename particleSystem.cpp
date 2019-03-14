#include "particleSystem.h"
#include <cstdlib>
#include <cmath>

ParticleSystem::ParticleSystem(int number):EntityTemplate(VertexArraySprite(sf::Quads,number*4)),particles(number),source(0,0){
	for(std::size_t i=0;i< s().vertices.getVertexCount();i++){
		s().vertices[i].color = sf::Color::Blue;
	}
}

void ParticleSystem::update(float seconds){
	for(std::size_t i=0;i<particles.size();i++){
		Particle& p = particles[i];
		p.lifetime-=seconds;
		if(p.lifetime<=0)
			resetParticle(i);
		
		s().vertices[i*4].position += p.velocity * seconds;
		s().vertices[i*4+1].position += p.velocity * seconds;
		s().vertices[i*4+2].position += p.velocity * seconds;
		s().vertices[i*4+3].position += p.velocity * seconds;
		p.velocity -= p.velocity*0.8f*seconds;
	}
}

void ParticleSystem::resetParticle(std::size_t index){
	float angle = (std::rand()%360)*(3.14f/180.f);
	float speed = (std::rand()%10)+5.f;
	particles[index].velocity = sf::Vector2f(speed*std::cos(angle),speed*std::sin(angle));
	particles[index].lifetime = 0.5+(1.f*std::rand())/RAND_MAX;
	s().vertices[index*4].position = source + sf::Vector2f(-4,-4);
	s().vertices[index*4+1].position = source + sf::Vector2f(4,-4);
	s().vertices[index*4+2].position = source + sf::Vector2f(4,4);
	s().vertices[index*4+3].position = source + sf::Vector2f(-4,4);
}