#ifndef EXAMPLE_ENTITY_H_GUARD
#define EXAMPLE_ENTITY_H_GUARD

#include "entity.h"
#include "spriteUSS.h"

class ExampleEntity: public Entity{
public:
	ExampleEntity(const SpriteSheet& spriteSheet,const Resources& resources):sprite(spriteSheet),motion(),hitbox(),animator() {}
	ExampleEntity(const SpriteSheet& spriteSheet,const Resources& resources,const Hitbox& hitbox):sprite(spriteSheet), motion(), hitbox(hitbox), animator() {}

	sf::Sprite* s() { return &sprite; }
	const sf::Sprite* s()const { return &sprite; }

	sf::Transformable* t() { return &sprite; }
	const sf::Transformable* t()const { return &sprite; }

	sf::Drawable* d() { return &sprite; }
	const sf::Drawable* d()const { return &sprite; }
	
	Motion* m() { return &motion; }
	const Motion* m()const { return &motion; }

	Hitbox* h() { return &hitbox; }
	const Hitbox* h()const { return &hitbox; }

	Animator* a() { return &animator; }
	const Animator* a()const { return &animator; }

	void setFrame(int frame){ sprite.setFrame(frame); }

private:

	//Entity components
	SpriteUSS sprite;
	Motion motion;
	Hitbox hitbox;
	Animator animator;

};

#endif