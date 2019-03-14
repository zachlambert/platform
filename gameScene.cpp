/* SIMPLE SCENE h

### PURPOSE ###
-Defines implementation of SimpleScene class, used for main game scene

*/

#include "gameScene.h"
#include "polygonShape.h"
#include "hitbox.h"
#include <vector>
#include <SFML/Graphics/Rect.hpp>

GameScene::GameScene(sf::RenderWindow& window,const Resources& resources):
		Scene(window,resources),
        player(resources.getSpriteSheet("rough_spritesheet.png"), resources),
		entity2(resources.getSpriteSheet("irregular_spritesheet.png")),
	    textEntity(),
	    testSound(),
		tileMap(resources.getTileMapData("example_tilemap")),
	    particles(800){
	
	entity2.s().setFrame(1);
	
	textEntity.setFont(resources.getFont("arial.ttf"));
	textEntity.setString("Some Text");
	textEntity.setPosition(200,200);
	textEntity.setCharacterSize(48);
	textEntity.setFillColor(sf::Color::Blue);
	textEntity.setStyle(sf::Text::Bold|sf::Text::Underlined);
	
	loadMusic(music,"example.wav");
	testSound.setBuffer(resources.getSoundBuffer("punch1.wav"));
	
	tileMap.t().setPosition(100,0);
	
	view.zoom(2.f);
}

void GameScene::handleEvent(sf::Event event){
	if(event.type == sf::Event::KeyPressed){
		if(event.key.code == sf::Keyboard::W){
			player.jump();
		}else if(event.key.code == sf::Keyboard::R){
			player.t().setPosition(1450,300);
			player.m().setVelocity(0,0);
		}else if(event.key.code == sf::Keyboard::LShift){
			player.startRunning();
		}else if(event.key.code == sf::Keyboard::U){
			entity2.s().setFrame(0);
		}else if(event.key.code == sf::Keyboard::I){
			entity2.s().setFrame(1);
		}else if(event.key.code == sf::Keyboard::O){
			entity2.s().setFrame(2);
		}else if(event.key.code == sf::Keyboard::P){
			entity2.s().setFrame(3);
		}else if(event.key.code == sf::Keyboard::L){
			testSound.play();
		}else if(event.key.code == sf::Keyboard::J){
			music.play();
		}else if(event.key.code == sf::Keyboard::K){
			music.stop();
		}
	}else if(event.type == sf::Event::KeyReleased){
		if(event.key.code == sf::Keyboard::LShift){
			player.stopRunning();
		}
	}
}

void GameScene::update(float seconds){

	//Update player velocity

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		player.moveLeft();
	}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		player.moveRight();
	}else{
		player.stopHorizontalMovement();
	}
	
	if(player.m().isInFreeFall()){
		if(player.a().isInAnimation("walk_rough")){
			player.a().pauseAnimation();
		}
		player.s().setFrame(8);
	}
	
	
	//Check if on ground
	player.t().move(0,5);
	if(!collideTileMap(tileMap.tm(),player.t(),player.h())){
		player.t().move(0,-5);
		player.m().setInFreeFall(true);
	}else{
		player.t().move(0,-5);
		player.m().setInFreeFall(false);
	}
	
	//Update entities

	player.update(seconds);

	//Move entity

	player.t().move(player.m().getDisplacement());

	if(collideTileMap(tileMap.tm(),player.t(),player.h())){
		player.t().move(-player.m().getDisplacement());
	
		static const float halfMinIncrement = 0.5f; //Min increment is 1, use half since this is what the increment would be after incrementing then halving.
		
		float moveIncrement = player.m().getDisplacement().x/2;
		float moved = 0;
		while(abs(moveIncrement)>halfMinIncrement){
			player.t().move(moveIncrement,0);
			if(collideTileMap(tileMap.tm(),player.t(),player.h())){
				player.t().move(-moveIncrement,0);
			}else{
				moved+=moveIncrement;
			}
			moveIncrement/=2;
		}
		if(abs(moveIncrement)<=halfMinIncrement){
			player.t().move(player.m().getDisplacement().x-moved,0);
			if(collideTileMap(tileMap.tm(),player.t(),player.h())){
				player.t().move(moved-player.m().getDisplacement().x,0);
				player.m().setVelocityX(0);
			}
		}else{
			player.m().setVelocityX(0);
		}
		
		moveIncrement = player.m().getDisplacement().y/2;
		moved = 0;
		while(abs(moveIncrement)>halfMinIncrement){
			player.t().move(0,moveIncrement);
			if(collideTileMap(tileMap.tm(),player.t(),player.h())){
				player.t().move(0,-moveIncrement);
			}else{
				moved+=moveIncrement;
			}
			moveIncrement/=2;
		}
		if(abs(moveIncrement)<=halfMinIncrement){
			player.t().move(0,player.m().getDisplacement().y-moved);
			if(collideTileMap(tileMap.tm(),player.t(),player.h())){
				player.t().move(0,moved-player.m().getDisplacement().y);
				player.m().setInFreeFall(false);
				player.m().setVelocityY(0);
			}
		}else{
			player.m().setVelocityY(0);
			player.m().setInFreeFall(false);
		}
	}
	
	entity2.t().setPosition(player.t().getPosition() + sf::Vector2f(0, 100));
	view.setCenter(player.t().getPosition());

	particles.ps().setSource(player.t().getPosition()+sf::Vector2f(0,-60));
	particles.update(seconds);

}
void GameScene::draw(sf::RenderTarget& target,sf::RenderStates states)const{
	target.draw(tileMap);
	target.draw(player);
	target.draw(entity2);
	target.draw(particles);
	target.draw(textEntity);
	drawHitbox(target,player.t(),player.h());
}