/* SIMPLE SCENE h

### PURPOSE ###
-Defines the SimpleScene class, which is the main game scene

@TODO: Change name of class
*/

#ifndef SIMPLE_SCENE_H_GUARD
#define SIMPLE_SCENE_H_GUARD

#include "scene.h"
#include "exampleEntity.h"
#include "tilemap.h"
#include "music.h"
#include "particleSystem.h"

class SimpleScene: public Scene{
public:
	SimpleScene(sf::RenderWindow&,const Resources&);
	
	virtual void handleEvent(sf::Event);
	virtual void update(float);
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const;
	
private:
	ExampleEntity entity;
	ExampleEntity entity2;
	sf::Text textEntity;
	sf::Sound testSound;
	sf::Music music;
	TileMap tileMap;
	ParticleSystem particles;
};

#endif