#ifndef PLAYER_H_GUARD
#define PLAYER_H_GUARD

#include "resources.h"
#include "entity.h"
#include "spriteSheetSprite.h"

class Player: public EntityTemplate<PhysicalEntity,SpriteSheetSprite>, public Animated{
public:
	Player(const SpriteSheet& spriteSheet, const Resources& resources);

	virtual void update(float seconds);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const { target.draw(s()); }

	//For Entity
	sf::Transformable& t() { return s(); }
	const sf::Transformable& t()const { return s(); }

	//For PhysicalEntity
	virtual void setOffGround();
	virtual void setOnGround();

	//Player specific functions

	void moveLeft();
	void moveRight();
	void stopHorizontalMovement();

	void jump();

	void startRunning();
	void stopRunning();

protected:

	virtual const sf::Drawable& getDrawable()const { return s(); }

private:

	const float WALK_SPEED = 600;
	const float RUN_SPEED = 1200;
	const float JUMP_SPEED = 1100;
	const float LARGE_JUMP_SPEED = 1800;

	float horizontalSpeed;

};

#endif