#ifndef TILEMAP_H_GUARD
#define TILEMAP_H_GUARD

#include <vector>
#include "entity.h"
#include "texturedVertexArraySprite.h"

class TileMap: public EntityTemplate<Entity,TexturedVertexArraySprite>{
public:
	TileMap(const TileMapData&);

	virtual void update(float seconds){} //Do nothing

	const TileMapData& getTileMapData()const{ return tileMapData; }

private:

	const TileMapData& tileMapData;
};

#endif