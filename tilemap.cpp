#include "tilemap.h"
#include <algorithm>

TileMapData::TileMapData(const std::string& name,const SpriteSheet& spriteSheet,const std::vector<int> tiles,
		const sf::Vector2u size,const sf::Vector2u tileSize,const std::vector<int> solidTiles):
			name(name),spriteSheet(spriteSheet),tiles(tiles),size(size),tileSize(tileSize),tileCount(0),solidTiles(solidTiles){
	for(std::vector<int>::const_iterator iter=tiles.begin();iter!=tiles.end();iter++){
		if(*iter!=-1){
			tileCount++;
		}
	}
}

const int TileMapData::getTileFrameIndex(int x,int y)const{
	int index = y*size.x+x;
	if(x<0 || x>= size.x || y<0 || y>=size.y){
		return -1;
	}else{
		return tiles[y*size.x+x];
	}
} 
const bool TileMapData::isSolid(int frameIndex)const{
	std::vector<int>::const_iterator iter = std::find(solidTiles.begin(),solidTiles.end(),frameIndex);
	return iter!=solidTiles.end();
}
const bool TileMapData::isSolid(int x,int y)const{
	return isSolid(getTileFrameIndex(x,y));
}

sf::Vector2f getTilePosition(int x,int y,const sf::Vector2u& tileSize){
	return sf::Vector2f(x*tileSize.x,y*tileSize.y);
}

TileMap::TileMap(const TileMapData& tilemap):
		TexturedVertexArraySprite(tilemap.getSpriteSheet().getTexture()),tileMapData(tilemap){
	
	//Set primitive type and number of vertices
	
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(tilemap.getTileCount()*4);
	
	//Add tiles to the vertex array
	
	sf::IntRect textureRect;
	sf::Vertex* quad;
	int tilesPlaced = 0;
	for(int y=0;y!=tilemap.getSize().y&&tilesPlaced<tilemap.getTileCount();y++){
		for(int x=0;x!=tilemap.getSize().x&&tilesPlaced<tilemap.getTileCount();x++){
			int frameIndex = tilemap.getTileFrameIndex(x,y);
			if(frameIndex!=-1){
				textureRect = tilemap.getSpriteSheet().getTextureRect(frameIndex);
				
				quad = &vertices[tilesPlaced*4];
				
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