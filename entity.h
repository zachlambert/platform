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

	-For any functionality which is likely to be required by multiple entities (such as animation),
	 functionality is implemented in a separate class, independent of entities, and then an object
	 is included in the entity. Access to this object can be provided by concise getter functions,
	 like with sf::Transformable.
	-Doing it this way allows for more modular code, and avoid the need for things like multiple
	 inheritance, while still allowing flexibility in what functions are provided.

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

#endif
