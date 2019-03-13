/* NEW ENTITY h//

### PURPOSE ###
-Defines NewEntity class (rename to Entity when implemented)

### NEW ENTITY INTERFACE ###

Constructor:
	Takes resources to store

getters:
	These are defined as return nullptr here, but will
	be overloaded in child classes, when they implement
	different funcionality
	-d	  | < These two may return a pointer to the same object. Would ideally just return a sprite, but some entities
	-t | < may use vertexArraySprite, which is a custom sprite, which extends drawable and transformable. 
	-m
	-h
	-a

Objects for these getters are classes, which may have some methods

Non-member functions are used to update entities instead of having internal update methods

Note: There are non-const and const methods of each of the s,t,d,m,h,a, because they need to be accessible for modification, and accessible in
const functions. For example, the draw function (overriding sf::Drawable draw), must be const, so all functions called in it must be const. Therefore,
if you want to access the transformable for position, you need to get a const pointer.

*/



#ifndef ENTITY_H_GUARD
#define ENTITY_H_GUARD

#include <SFML/Graphics.hpp>

#include "motion.h"
#include "hitbox.h"
#include "animator.h"

#include "spriteUSS.h"
#include "particleSystem.h"
#include "tilemap.h"

class Entity {
public:

	Entity() {}

	virtual void update(float seconds) = 0;

	virtual sf::Drawable& d() = 0;
	virtual const sf::Drawable& d()const = 0;

	virtual sf::Transformable& t() = 0;
	virtual const sf::Transformable& t()const = 0;

};


class SpriteEntity : public Entity {
public:
	SpriteEntity(const SpriteSheet& spriteSheet, const Resources& resources) :sprite(spriteSheet), motion(), hitbox(), animator() {}
	SpriteEntity(const SpriteSheet& spriteSheet, const Resources& resources, const Hitbox& hitbox) :sprite(spriteSheet), motion(), hitbox(hitbox), animator() {}

	void update(float seconds);

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const { return sprite; }

	sf::Drawable& d() { return sprite; }
	const sf::Drawable& d()const { return sprite; }

	SpriteUSS& s() { return sprite; }
	const SpriteUSS& s()const { return sprite; }

	Motion& m() { return motion; }
	const Motion& m()const { return motion; }

	Hitbox& h() { return hitbox; }
	const Hitbox& h()const { return hitbox; }

	Animator& a() { return animator; }
	const Animator& a()const { return animator; }

private:

	//Entity components
	SpriteUSS sprite;
	Motion motion;
	Hitbox hitbox;
	Animator animator;

};


class ParticleSystemEntity : public Entity {
public:
	ParticleSystemEntity(int count) :sprite(count) {}

	void update(float seconds) { sprite.update(seconds); }

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const { return sprite; }

	sf::Drawable& d() { return sprite; }
	const sf::Drawable& d()const { return sprite; }

	ParticleSystem& ps() { return sprite; }

private:

	ParticleSystem sprite;

};


class TileMapEntity: public Entity{
public:
	TileMapEntity(const TileMapData& tileMapData):tileMap(tileMapData){}

	void update(float seconds){} //Do nothing

	sf::Transformable& t(){ return tileMap; }
	const sf::Transformable& t()const{ return tileMap; }

	sf::Drawable& d(){ return tileMap; }
	const sf::Drawable& d()const{ return tileMap; }

	TileMap& tm(){ return tileMap; }
	const TileMap& tm()const{ return tileMap; }

private:
	TileMap tileMap;
};

#endif
