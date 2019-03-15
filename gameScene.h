/* SIMPLE SCENE h

### PURPOSE ###
-Defines the SimpleScene class, which is the main game scene

@TODO: Change name of class
*/

#ifndef SIMPLE_SCENE_H_GUARD
#define SIMPLE_SCENE_H_GUARD

#include "scene.h"
#include "entity.h"
#include "player.h"
#include "tileMap.h"
#include "particleSystem.h"
#include "music.h"
#include "resources.h"

class GameScene: public Scene{
public:
	GameScene(sf::RenderWindow&,const Resources&);
	
	virtual void handleEvent(sf::Event);
	virtual void update(float);
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const;
	
private:
	Player player;
	TileMap tileMap;
	ParticleSystem particleSystem;
	sf::Sound punchSound;

	void moveEntities();
	void moveEntity(PhysicalEntity& entity);
	bool collideSolid(PhysicalEntity& entity);

};

#endif