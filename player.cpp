
#include "player.h"

Player::Player(const SpriteSheet& spriteSheet, const Resources& resources):
	    SpriteEntity(spriteSheet),
	    hitbox(),
	    motion(),
	    animator(resources),
	    horizontalSpeed(WALK_SPEED){

	t().setPosition(200, 100 + s().getLocalBounds().height);
	m().setInFreeFall(true);
	s().setScale(0.5f, 0.5f);
	a().setAnimationSpeed(1.4);
	h() = Hitbox(-s().getLocalBounds().width / 2, -s().getLocalBounds().height / 2, s().getLocalBounds().width, s().getLocalBounds().height);
	s().setFrame(10);
}

void Player::update(float seconds) {

	m().update(seconds);

	a().update(seconds);
	if (!a().isPaused()) {
		s().setFrame(a().getFrame());
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
		m().setInFreeFall(true);
	}
}

void Player::startRunning(){
	a().setAnimationSpeed(2.8);
	horizontalSpeed = RUN_SPEED;
}

void Player::stopRunning(){
	a().setAnimationSpeed(1.4);
	horizontalSpeed = WALK_SPEED;
}

void Player::moveLeft(){
	if (!m().isInFreeFall()) {
		if (a().isInAnimation("walk_rough")) {
			a().resumeAnimation();
		}
		else {
			a().startAnimation("walk_rough", true);
		}
	}
	s().setScale(-0.5f, 0.5f);
	m().setVelocityX(-horizontalSpeed);
}

void Player::moveRight(){
	if (!m().isInFreeFall()) {
		if (a().isInAnimation("walk_rough")) {
			a().resumeAnimation();
		}
		else {
			a().startAnimation("walk_rough", true);
		}
	}
	s().setScale(0.5f, 0.5f);
	m().setVelocityX(horizontalSpeed);
}

void Player::stopHorizontalMovement(){
	m().setVelocityX(0);
	a().pauseAnimation();
	if (!m().isInFreeFall()) {
		s().setFrame(10);
	}
}