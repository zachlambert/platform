#include "tileMap.h"
#include <algorithm>

sf::Vector2f getTilePosition(int x,int y,const sf::Vector2u& tileSize){
	return sf::Vector2f(x*tileSize.x,y*tileSize.y);
}

TileMap::TileMap(const TileMapData& tilemap):
		EntityTemplate(TexturedVertexArraySprite(tilemap.getSpriteSheet().getTexture())),tileMapData(tilemap){
	
	//Set primitive type and number of vertices
	
	s().vertices.setPrimitiveType(sf::Quads);
	s().vertices.resize(tilemap.getTileCount()*4);
	
	//Add tiles to the vertex array
	
	sf::IntRect textureRect;
	sf::Vertex* quad;
	int tilesPlaced = 0;
	for(int y=0;y!=tilemap.getSize().y&&tilesPlaced<tilemap.getTileCount();y++){
		for(int x=0;x!=tilemap.getSize().x&&tilesPlaced<tilemap.getTileCount();x++){
			int frameIndex = tilemap.getTileFrameIndex(x,y);
			if(frameIndex!=-1){
				textureRect = tilemap.getSpriteSheet().getTextureRect(frameIndex);
				
				quad = &s().vertices[tilesPlaced*4];
				
				quad[0].position = getTilePosition(x,y,tilemap.getTileSize());
				quad[1].position = getTilePosition(x+1,y,tilemap.getTileSize());
				quad[2].position = getTilePosition(x+1,y+1,tilemap.getTileSize());
				quad[3].position = getTilePosition(x,y+1,tilemap.getTileSize());
				
				quad[0].texCoords = sf::Vector2f(textureRect.left,textureRect.top);
				quad[1].texCoords = sf::Vector2f(textureRect.left+textureRect.width,textureRect.top);
				quad[2].texCoords = sf::Vector2f(textureRect.left+textureRect.width,textureRect.top+textureRect.height);
				quad[3].texCoords = sf::Vector2f(textureRect.left,textureRect.top+textureRect.height);
				
				tilesPlaced++;
			}
		}
	}
	
}