/* SIMPLE SCENE h

### PURPOSE ###
-Defines implementation of SimpleScene class, used for main game scene

*/

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include "gameScene.h"
#include "polygonSprite.h"
#include "collision.h"
#include "resourceNames.h"

GameScene::GameScene(sf::RenderWindow& window,const Resources& resources):
		Scene(window,resources),
        player(resources.getSpriteSheet(resource::SpriteSheet::roughSpriteSheet), resources),
		tileMap(resources.getTileMapData(resource::TileMap::example)),
        particleSystem(100),
        punchSound(){
	
	tileMap.t().setPosition(100,0);
	
	punchSound.setBuffer(resources.getSoundBuffer(resource::Sound::punch1));
	
	view.zoom(2.f);
}

void GameScene::handleEvent(sf::Event event){

	//Key press events
	if(event.type == sf::Event::KeyPressed){
		switch(event.key.code){
		case sf::Keyboard::A:
			player.moveLeft();
			break;
		case sf::Keyboard::D:
			player.moveRight();
			break;
		case sf::Keyboard::W:
			player.jump();
			break;
		case sf::Keyboard::R:
			player.t().setPosition(1450,300);
			break;
		case sf::Keyboard::LShift:
			player.startRunning();
			break;
		case sf::Keyboard::Space:
			punchSound.play();
			break;
		}

	//Key release events
	}else if(event.type == sf::Event::KeyReleased){
		switch(event.key.code){
		case sf::Keyboard::A:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				player.moveRight();
			}else{
				player.stopHorizontalMovement();
			}
			break;
		case sf::Keyboard::D:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopHorizontalMovement();
			}
			break;
		case sf::Keyboard::LShift:
			player.stopRunning();
			break;
		}

	//Mouse press events
	}else if(event.type == sf::Event::MouseButtonPressed){
		if(event.mouseButton.button == sf::Mouse::Left){
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			sf::Vector2f position = window.mapPixelToCoords(mousePosition);
			player.t().setPosition(position);
		}else if(event.mouseButton.button == sf::Mouse::Right){

		}

	//Mouse release events
	}else if(event.type == sf::Event::MouseButtonReleased){
		switch(event.mouseButton.button){
		case sf::Mouse::Left:
			break;
		case sf::Mouse::Right:
			break;
		}
	}
}

void GameScene::update(float seconds){

	//Update entities, excluding movement

	player.update(seconds);
	particleSystem.update(seconds);
	particleSystem.setSource(player.t().getPosition());

	//Move the entities

	moveEntities();

	//Check for collisions between certain entities
	//(eg: check for collisions between enemies and player)

	//Update view
	view.setCenter(player.t().getPosition());
}

void GameScene::draw(sf::RenderTarget& target,sf::RenderStates states)const{
	target.draw(tileMap);
	target.draw(player);
	target.draw(particleSystem);
	//drawHitbox(target,player.t(),player.h());
}

void GameScene::moveEntities(){
	moveEntity(player);
}

void GameScene::moveEntity(PhysicalEntity& physical){

	physical.t().move(physical.m().getDisplacement());

	if (collideSolid(physical)) {
		physical.t().move(-physical.m().getDisplacement());

		static const float halfMinIncrement = 0.5f; //Min increment is 1, use half since this is what the increment would be after incrementing then halving.

		float moveIncrement = physical.m().getDisplacement().x / 2;
		float moved = 0;
		while (abs(moveIncrement) > halfMinIncrement) {
			physical.t().move(moveIncrement, 0);
			if (collideSolid(physical)) {
				physical.t().move(-moveIncrement, 0);
			}
			else {
				moved += moveIncrement;
			}
			moveIncrement /= 2;
		}
		if (abs(moveIncrement) <= halfMinIncrement) {
			physical.t().move(physical.m().getDisplacement().x - moved, 0);
			if (collideSolid(physical)) {
				physical.t().move(moved - physical.m().getDisplacement().x, 0);
				//physical.m().setVelocityX(0);
			}
		}
		else {
			//physical.m().setVelocityX(0);
		}

		moveIncrement = physical.m().getDisplacement().y / 2;
		moved = 0;
		while (abs(moveIncrement) > halfMinIncrement) {
			physical.t().move(0, moveIncrement);
			if (collideSolid(physical)) {
				physical.t().move(0, -moveIncrement);
			}
			else {
				moved += moveIncrement;
			}
			moveIncrement /= 2;
		}
		if (abs(moveIncrement) <= halfMinIncrement) {
			physical.t().move(0, physical.m().getDisplacement().y - moved);
			if (collideSolid(physical)) {
				physical.t().move(0, moved - physical.m().getDisplacement().y);
				physical.m().setInFreeFall(false);
				physical.m().setVelocityY(0);
			}
		}
		else {
			physical.m().setVelocityY(0);
			physical.setOnGround();
		}
	}

	//Check if the physical is on ground
	physical.t().move(0, 5);
	if (!collideSolid(physical)){
		physical.t().move(0, -5);
		physical.setOffGround();
	}
	else {
		physical.t().move(0, -5);
		physical.setOnGround();
	}

}

bool GameScene::collideSolid(PhysicalEntity& physical){
	//Currently only checks for collision with the tilemap, but other solid entities may be included

	return collideTileMap(tileMap, physical.t(), physical.h());
}