/* SIMPLE SCENE h

### PURPOSE ###
-Defines implementation of SimpleScene class, used for main game scene

*/

#include "simpleScene.h"
#include "polygonShape.h"
#include "hitbox.h"
#include <vector>
#include <SFML/Graphics/Rect.hpp>

SimpleScene::SimpleScene(sf::RenderWindow& window,const Resources& resources):
		Scene(window,resources),entity(resources.getSpriteSheet("rough_spritesheet.png"),resources),
		entity2(resources.getSpriteSheet("irregular_spritesheet.png"),resources),textEntity(),testSound(),
		tileMap(resources.getTileMapData("example_tilemap")),particles(120){
	
	entity.t()->setPosition(200,100 + entity.s()->getLocalBounds().height);
	entity.m()->setInFreeFall(true);
	entity.s()->setScale(0.5f,0.5f);
	entity.a()->setAnimationSpeed(1.4);
	*entity.h() = Hitbox(-entity.s()->getLocalBounds().width/2, -entity.s()->getLocalBounds().height/2, entity.s()->getLocalBounds().width, entity.s()->getLocalBounds().height);
	entity.setFrame(10);
	
	entity2.setFrame(1);
	
	textEntity.setFont(resources.getFont("arial.ttf"));
	textEntity.setString("Some Text");
	textEntity.setPosition(200,200);
	textEntity.setCharacterSize(48);
	textEntity.setFillColor(sf::Color::Blue);
	textEntity.setStyle(sf::Text::Bold|sf::Text::Underlined);
	
	loadMusic(music,"example.wav");
	testSound.setBuffer(resources.getSoundBuffer("punch1.wav"));
	
	tileMap.setPosition(100,0);
	
	view.zoom(2.f);
}

void SimpleScene::handleEvent(sf::Event event){
	if(event.type == sf::Event::KeyPressed){
		if(event.key.code == sf::Keyboard::W){
			if(!entity.m()->isInFreeFall()){
				entity.m()->setVelocityY(-1100);
				entity.m()->setInFreeFall(true);
			}
		}else if(event.key.code == sf::Keyboard::R){
			entity.t()->setPosition(1450,300);
			entity.m()->setVelocity(0,0);
		}else if(event.key.code == sf::Keyboard::LShift){
			entity.a()->setAnimationSpeed(2.8);
		}else if(event.key.code == sf::Keyboard::U){
			entity2.setFrame(0);
		}else if(event.key.code == sf::Keyboard::I){
			entity2.setFrame(1);
		}else if(event.key.code == sf::Keyboard::O){
			entity2.setFrame(2);
		}else if(event.key.code == sf::Keyboard::P){
			entity2.setFrame(3);
		}else if(event.key.code == sf::Keyboard::L){
			testSound.play();
		}else if(event.key.code == sf::Keyboard::J){
			music.play();
		}else if(event.key.code == sf::Keyboard::K){
			music.stop();
		}
	}else if(event.type == sf::Event::KeyReleased){
		if(event.key.code == sf::Keyboard::LShift){
			entity.a()->setAnimationSpeed(1.4);
		}
	}
}

void SimpleScene::update(float seconds){

	//Respond to user input

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		if(!entity.m()->isInFreeFall()){
			if(entity.a()->isInAnimation("walk_rough")){
				entity.a()->resumeAnimation();
			}else{
				entity.a()->startAnimation(&resources.getAnimation("walk_rough"),true);
			}
		}
		entity.s()->setScale(-0.5f,0.5f);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
			entity.m()->setVelocityX(-1200);
		}else{
			entity.m()->setVelocityX(-600);	
		}
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		if(!entity.m()->isInFreeFall()){
			if(entity.a()->isInAnimation("walk_rough")){
				entity.a()->resumeAnimation();
			}else{
				entity.a()->startAnimation(&resources.getAnimation("walk_rough"),true);
			}
		}
		entity.s()->setScale(0.5f,0.5f);
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
			entity.m()->setVelocityX(1200);
		}else{
			entity.m()->setVelocityX(600);	
		}
	}else{
		entity.m()->setVelocityX(0);
		entity.a()->pauseAnimation();
		if(!entity.m()->isInFreeFall()){
			entity.setFrame(10);
		}
	}
	
	if(entity.m()->isInFreeFall()){
		if(entity.a()->isInAnimation("walk_rough")){
			entity.a()->pauseAnimation();
		}
		entity.setFrame(8);
	}
	
	
	//Check if on ground
	entity.t()->move(0,5);
	if(!collideTileMap(tileMap,*entity.t(),*entity.h())){
		entity.t()->move(0,-5);
		entity.m()->setInFreeFall(true);
	}else{
		entity.t()->move(0,-5);
		entity.m()->setInFreeFall(false);
	}
	
	//Move and check collisions

	entity.m()->update(seconds);
	entityMove(entity,seconds);

	if(collideTileMap(tileMap,*entity.t(),*entity.h())){
		entity.t()->move(-entity.m()->getDisplacement());
	
		static const float halfMinIncrement = 0.5f; //Min increment is 1, use half since this is what the increment would be after incrementing then halving.
		
		float moveIncrement = entity.m()->getDisplacement().x/2;
		float moved = 0;
		while(abs(moveIncrement)>halfMinIncrement){
			entity.t()->move(moveIncrement,0);
			if(collideTileMap(tileMap,*entity.t(),*entity.h())){
				entity.t()->move(-moveIncrement,0);
			}else{
				moved+=moveIncrement;
			}
			moveIncrement/=2;
		}
		if(abs(moveIncrement)<=halfMinIncrement){
			entity.t()->move(entity.m()->getDisplacement().x-moved,0);
			if(collideTileMap(tileMap,*entity.t(),*entity.h())){
				entity.t()->move(moved-entity.m()->getDisplacement().x,0);
				entity.m()->setVelocityX(0);
			}
		}else{
			entity.m()->setVelocityX(0);
		}
		
		moveIncrement = entity.m()->getDisplacement().y/2;
		moved = 0;
		while(abs(moveIncrement)>halfMinIncrement){
			entity.t()->move(0,moveIncrement);
			if(collideTileMap(tileMap,*entity.t(),*entity.h())){
				entity.t()->move(0,-moveIncrement);
			}else{
				moved+=moveIncrement;
			}
			moveIncrement/=2;
		}
		if(abs(moveIncrement)<=halfMinIncrement){
			entity.t()->move(0,entity.m()->getDisplacement().y-moved);
			if(collideTileMap(tileMap,*entity.t(),*entity.h())){
				entity.t()->move(0,moved-entity.m()->getDisplacement().y);
				entity.m()->setInFreeFall(false);
				entity.m()->setVelocityY(0);
			}
		}else{
			entity.m()->setVelocityY(0);
			entity.m()->setInFreeFall(false);
		}
	}
	
	entity2.t()->setPosition(entity.t()->getPosition() + sf::Vector2f(0, 100));
	view.setCenter(entity.t()->getPosition());

	particles.setSource(entity.t()->getPosition());
	particles.update(seconds);

	//Update animations

	entityUpdateAnimation(entity,seconds);

}
void SimpleScene::draw(sf::RenderTarget& target,sf::RenderStates states)const{
	target.draw(tileMap);
	target.draw(*entity.d());
	target.draw(*entity2.d());
	target.draw(particles);
	drawHitbox(target,*entity.t(),*entity.h());
}