#ifndef TILEMAP_H_GUARD
#define TILEMAP_H_GUARD

#include "entity.h"

class TileMap : public EntityTemplate<Entity, TileMapSprite> {
public:
	TileMap(const TileMapData& tileMapData) :EntityTemplate(TileMapSprite(tileMapData)) {}

	virtual void update(float seconds) {} //Do nothing

};

#endif
