#ifndef TILEMAP_H_GUARD
#define TILEMAP_H_GUARD

#include <vector>
#include "entity.h"
#include "texturedVertexArraySprite.h"
#include "resourceNames.h"

//Note: I've decided not to make a tilemap an entity, because:
//1. It doesn't need to be. It is always going to be defined as a TileMap, there is no need for polymorphism.
//2. It isn't updated.
//3. It isn't drawn all at once. Different layers need to be drawn separately, so it doesn't make sense for it to have a single draw function.

class TileMap: public sf::Transformable{

public:
	TileMap(const TileMapData&);

	virtual void update(float seconds){} //Do nothing

	void TileMap::buildTileLayer(TileMapLayer, const std::vector<int>& tiles);
	void drawTileLayer(TileMapLayer, sf::RenderTarget&, sf::RenderStates)const;

	const TileMapData& getTileMapData()const{ return tileMapData; }

private:

	const TileMapData& tileMapData;

	std::vector<TexturedVertexArraySprite> tileLayers;

};

#endif