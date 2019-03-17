#include "tileMap.h"
#include <algorithm>

sf::Vector2f getTilePosition(int x,int y,const sf::Vector2u& tileSize){
	return sf::Vector2f(x*tileSize.x,y*tileSize.y);
}

TileMap::TileMap(const TileMapData& tileMapData):tileLayers(static_cast<int>(TileMapLayer::NUMBER_OF_LAYERS)),tileMapData(tileMapData){

	//Add tiles to the vertex array
	buildTileLayer(TileMapLayer::solid,tileMapData.getSolidTiles());
	buildTileLayer(TileMapLayer::foreground,tileMapData.getForegroundTiles());
	buildTileLayer(TileMapLayer::background, tileMapData.getBackgroundTiles());
}

void TileMap::buildTileLayer(TileMapLayer layerNumber, const std::vector<int>& tiles){

	TexturedVertexArraySprite sprite = TexturedVertexArraySprite(tileMapData.getSpriteSheet().getTexture());

	sprite.vertices.setPrimitiveType(sf::Quads);
	sprite.vertices.resize(tileMapData.getTileCount()*4);

	sf::IntRect textureRect;
	sf::Vertex* quad;
	int tileNumber = 0;

	for(int y=0;y!=tileMapData.getSize().y;y++){
		for(int x=0;x!=tileMapData.getSize().x;x++){
			int frameIndex = tileMapData.getTileFrameIndex(tiles,x,y);

			if(frameIndex!=0){
				textureRect = tileMapData.getSpriteSheet().getTextureRect(frameIndex-1);
				
				quad = &sprite.vertices[tileNumber*4];
				
				quad[0].position = getTilePosition(x,y,tileMapData.getTileSize());
				quad[1].position = getTilePosition(x+1,y, tileMapData.getTileSize());
				quad[2].position = getTilePosition(x+1,y+1, tileMapData.getTileSize());
				quad[3].position = getTilePosition(x,y+1, tileMapData.getTileSize());
				
				quad[0].texCoords = sf::Vector2f(textureRect.left,textureRect.top);
				quad[1].texCoords = sf::Vector2f(textureRect.left+textureRect.width,textureRect.top);
				quad[2].texCoords = sf::Vector2f(textureRect.left+textureRect.width,textureRect.top+textureRect.height);
				quad[3].texCoords = sf::Vector2f(textureRect.left,textureRect.top+textureRect.height);

				tileNumber++;
			}
		}
	}

	tileLayers[static_cast<int>(layerNumber)] = sprite;
	
}

void TileMap::drawTileLayer(TileMapLayer tileMapLayer, sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(tileLayers[static_cast<int>(tileMapLayer)],states);
}