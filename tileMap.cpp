#include "tileMap.h"
#include <algorithm>

sf::Vector2f getTilePosition(int x,int y,const sf::Vector2u& tileSize){
	return sf::Vector2f(x*tileSize.x,y*tileSize.y);
}

TileMap::TileMap(const TileMapData& tileMapData):
		EntityTemplate(TexturedVertexArraySprite(tileMapData.getSpriteSheet().getTexture())),tileMapData(tileMapData){
	
	//Set primitive type and number of vertices
	
	s().vertices.setPrimitiveType(sf::Quads);
	s().vertices.resize(tileMapData.getTileCount()*4*3); //3 layers
	
	//Add tiles to the vertex array
	buildTileLayer(tileMapData.getBackgroundTiles(),0);
	buildTileLayer(tileMapData.getSolidTiles(),1);
	buildTileLayer(tileMapData.getForegroundTiles(),2);
}

void TileMap::buildTileLayer(const std::vector<int>& tiles, int layerNumber){

	sf::IntRect textureRect;
	sf::Vertex* quad;
	int tileNumber = layerNumber*tileMapData.getTileCount();

	for(int y=0;y!=tileMapData.getSize().y;y++){
		for(int x=0;x!=tileMapData.getSize().x;x++){
			int frameIndex = tileMapData.getTileFrameIndex(tiles,x,y);

			if(frameIndex!=0){
				textureRect = tileMapData.getSpriteSheet().getTextureRect(frameIndex-1);
				
				quad = &s().vertices[tileNumber*4];
				
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
	
}