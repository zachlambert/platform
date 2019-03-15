
#include "player.h"

Player::Player(const SpriteSheet& spriteSheet, const Resources& resources):
	    EntityTemplate(SpriteSheetSprite(spriteSheet)),
	    Animated(resources),
	    horizontalSpeed(WALK_SPEED){

	t().setPosition(200, 100 + s().getLocalBounds().height);
	m().setInFreeFall(true);
	s().setScale(0.5f, 0.5f);
	a().setAnimationSpeed(1.4);
	hitbox = Hitbox(-s().getLocalBounds().width / 2, -s().getLocalBounds().height / 2, s().getLocalBounds().width, s().getLocalBounds().height);
	s().setFrame(10);
}

void Player::update(float seconds) {

	//Update motion
	m().update(seconds);

	//Update animator
	a().update(seconds);
	if (!a().isPaused()) {
		s().setFrame(a().getFrame());
	}
}

void Player::moveLeft() {
	if (!m().isInFreeFall()) {
		if (a().isInAnimation(resource::Animation::walkRough)) {
			a().resumeAnimation();
		}else{
			a().startAnimation(resource::Animation::walkRough, true);
		}
	}
	s().setScale(-0.5f, 0.5f);
	m().setVelocityX(-horizontalSpeed);
}

void Player::moveRight() {
	if (!m().isInFreeFall()) {
		if (a().isInAnimation(resource::Animation::walkRough)) {
			a().resumeAnimation();
		}else{
			a().startAnimation(resource::Animation::walkRough, true);
		}
	}
	s().setScale(0.5f, 0.5f);
	m().setVelocityX(horizontalSpeed);
}

void Player::stopHorizontalMovement() {
	m().setVelocityX(0);
	a().pauseAnimation();
	if (!m().isInFreeFall()) {
		s().setFrame(10);
	}
}

void Player::jump(){
	if (!m().isInFreeFall()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			m().setVelocityY(-LARGE_JUMP_SPEED);
		}
		else {
			m().setVelocityY(-JUMP_SPEED);
		}
		setOffGround();
	}
}

void Player::setOffGround(){
	m().setInFreeFall(true);

	//Update appearance, stop walking animation
	if (a().isInAnimation(resource::Animation::walkRough)) {
		a().pauseAnimation();
	}
	s().setFrame(8);
}

void Player::setOnGround(){
	m().setInFreeFall(false);

	//Update appearance, resume walking animation if moving, otherwise
	//set frame to the stationary frame
	if(m().getVelocity().x!=0){
		if (a().isInAnimation(resource::Animation::walkRough)) {
			a().resumeAnimation();
		}else{
			a().startAnimation(resource::Animation::walkRough, true);
		}
	}else{
		s().setFrame(10);
	}
}

void Player::startRunning(){
	a().setAnimationSpeed(2.8);
	horizontalSpeed = RUN_SPEED;
	m().setVelocityX(m().getVelocity().x*(RUN_SPEED / WALK_SPEED));
}

void Player::stopRunning(){
	a().setAnimationSpeed(1.4);
	horizontalSpeed = WALK_SPEED;
	m().setVelocityX(m().getVelocity().x*(WALK_SPEED/RUN_SPEED));
}