/* ENTITY h// 

An entity is defined as follows:
	-It can be drawn
	-It can be transformed
	-It can be updated

There is also the need to define a physical entity, which:
	-Has a hitbox
	-Has a motion object (so can return a displacement)
	This is so that physical entities can be passed
	to functions and manipulated.

The drawing and transforming functionality it implemented
by a sprite.
A sprite is defined as: something which extends sf::Transformable
and sf::Drawable.

In order for it to be possible to define an Entity, which has access
to an sf::Drawable and sf::Transformable:
	-The entity class has virtual functions for returning an sf::Transformable&
	 and sf::Drawable&, called t() and d(), which must be defined in a child class.
	-The child class will hold a sprite (not extend it) and return a reference to
	 this sprite in both the classes.
	-It will also provide an s() function to provide reference to the sprite itself.
	-To avoid having to define this for each (Entity/PhysicalEntity) and (Sprite) combination,
	 an EntityTemplate is defined.

Further functionality, which just add to the entity and don't change its identity, can be
implemented by extending classes like "Animated", which hold an Animator object, and provide
access to this object through a concise getter, a().

Although t(), d(), s() return the same thing, it is necessary for the separation.
You can't specify a type as something which extends sf::Drawable and sf::Transformable, so in
order to get access to a type which does this, you need to provide functions for accessing each part.
You also need the s() function, since the entity might need to access the specific sprite, such as
for changing frames with a SpriteSheetSprite.

Finally, for the EntityTemplate<E,S>, E and S can be defined as follows:
	-E: The Base Entity. This is what the entity identifies as. (Allows passing the entity to functions which require certain functionality from it)
	-S: The Sprite. This is what the entity is implemented using. (Returned in the t(), d(), s() functions)

*/

#ifndef ENTITY_H_GUARD
#define ENTITY_H_GUARD

#include <SFML/Graphics.hpp>

#include "motion.h"
#include "hitbox.h"
#include "animator.h"
#include "resources.h"

class Entity: public sf::Drawable{
public:

	Entity() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const { target.draw(d()); }
	virtual void update(float seconds) = 0;

	virtual sf::Transformable& t() = 0;
	virtual const sf::Transformable& t()const = 0;
	virtual const sf::Drawable& d()const = 0;
};


class PhysicalEntity: public Entity{
public:
	PhysicalEntity():hitbox(),motion(){}

	Hitbox& h() { return hitbox; }
	const Hitbox& h()const { return hitbox; }
	Motion& m() { return motion; }
	const Motion& m()const { return motion; }

	virtual void setOnGround() = 0;
	virtual void setOffGround() = 0;

protected:
	Hitbox hitbox;
	Motion motion;

};

//Instead of creating a class for each Entity/PhysicalEntity and Sprite combination, a template
//can be used. Note: By sprite, I mean anything which extends transformable and drawable.
template <class E, class S>
class EntityTemplate: public E{
public:
	EntityTemplate(S sprite):E(),sprite(sprite){}

	sf::Transformable& t() { return sprite; }
	const sf::Transformable& t()const { return sprite; }
	const sf::Drawable& d()const { return sprite; }

	S& s(){ return sprite; }
	const S& s()const{ return sprite; }

private:
	S sprite;
};

//This can just be inherited by anything that wants to keep track of an animation and 
//retrieve the current frame. It doesn't directly manipulate anything itself.
class Animated {
public:
	Animated(const Resources& resources) :animator(resources) {}

	Animator& a() { return animator; }
	const Animator& a()const { return animator; }

protected:
	Animator animator;
};


#endif
