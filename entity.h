/* ENTITY h//

### PURPOSE ###
-Defines the parent Entity class and some core child entity classes

### ENTITY CLASS ###

An entity is something which:
	-Can be drawn
	-Can be transformed
	-Can run internal logic to update its state
	-Can implement additional functionality for specific entities
	
I have decided to structure the entity classes as follows:

	-Entity extends sf::Drawable, such that you can call target.draw(entity)
	-Entity defines a virtual getDrawable() function, which must be overloaded.
	 Child classes will contain the object they wish to draw, they won't extend it.
	 Then, access to this object is provided by overloading the getDrawable() function,
	 which the parent Entity class used in its draw(...) function.

	-Entity defines a virtual update(float seconds) function which must be
	 overloaded. (Can be overloaded with an empty function if not using it)
	
	-Entity defines a virtual t() function, which is a concise getter function for
	 an sf::Transformable. This allows access to sf::Transformable functions without having
	 to redefine them in entity.

	-For additional functionality, non-entity classes are defined, such as "Physical" and "Animated".
	 These can then be inherited by entities, to include the functionality.

For the Physical class, it redefines the t() function as a virtual function and requires that the child
entity redefines it. This is required in order to provide the Physical class with access to the sf::Transformable.
The Physical class is required, for passing entities to functions for updating movement and checking collisions.

The t() function can be defined using the t() function defined in the SpriteEntity, CompountSpriteEntity, etc... :
virtual sf::Transformable& t(){ return SpriteEntity::t(); }
-somehow this works

*/



#ifndef ENTITY_H_GUARD
#define ENTITY_H_GUARD

#include <SFML/Graphics.hpp>

#include "motion.h"
#include "hitbox.h"
#include "animator.h"

#include "spriteUSS.h"
#include "compoundSprite.h"
#include "particleSystem.h"
#include "tilemap.h"

class Entity: public sf::Drawable{
public:

	Entity() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const{ target.draw(getDrawable()); }

	virtual void update(float seconds) = 0;

	virtual sf::Transformable& t() = 0;
	virtual const sf::Transformable& t()const = 0;

private:
	
	virtual const sf::Drawable& getDrawable()const = 0;

};


class SpriteEntity : public Entity {
public:
	SpriteEntity(const SpriteSheet& spriteSheet) :sprite(spriteSheet){}

	virtual void update(float seconds){} //Do nothing

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const { return sprite; }

	SpriteUSS& s() { return sprite; }
	const SpriteUSS& s()const { return sprite; }

private:

	virtual const sf::Drawable& getDrawable()const{ return sprite; }

	SpriteUSS sprite;

};

class CompoundSpriteEntity: public Entity{
public:
	CompoundSpriteEntity():sprite(){}

	virtual void update(float seconds){} //Do nothing

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const{ return sprite; }

	CompoundSprite& cs(){ return sprite; }

private:

	virtual const sf::Drawable& getDrawable()const{ return sprite; }

	CompoundSprite sprite;
};

class ParticleSystemEntity : public Entity {
public:
	ParticleSystemEntity(int count) :particleSystem(count) {}

	virtual void update(float seconds) { particleSystem.update(seconds); }

	sf::Transformable& t() { return particleSystem; }
	const sf::Transformable& t()const { return particleSystem; }

	ParticleSystem& ps() { return particleSystem; }

private:

	virtual const sf::Drawable& getDrawable()const { return particleSystem; }

	ParticleSystem particleSystem;

};


class TileMapEntity: public Entity{
public:
	TileMapEntity(const TileMapData& tileMapData):tileMap(tileMapData){}

	virtual void update(float seconds){} //Do nothing

	sf::Transformable& t(){ return tileMap; }
	const sf::Transformable& t()const{ return tileMap; }

	TileMap& tm(){ return tileMap; }
	const TileMap& tm()const{ return tileMap; }

private:

	virtual const sf::Drawable& getDrawable()const { return tileMap; }

	TileMap tileMap;
};


class Physical{
public:
	Physical():hitbox(),motion(){}


	virtual sf::Transformable& t()=0;
	Hitbox& h(){ return hitbox; }
	const Hitbox& h()const{ return hitbox; }
	Motion& m(){ return motion; }
	const Motion& m()const{ return motion; } 

	virtual void setOnGround()=0;
	virtual void setOffGround()=0;

protected:
	Hitbox hitbox;
	Motion motion;

};

class Animated {
public:
	Animated(const Resources& resources) :animator(resources) {}

	Animator& a() { return animator; }
	const Animator& a()const { return animator; }

protected:
	Animator animator;
};


#endif
