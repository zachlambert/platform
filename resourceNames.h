#ifndef RESOURCE_NAMES_H_GUARD
#define RESOURCE_NAMES_H_GUARD

enum class TileMapLayer{
	solid,
	foreground,
	background,
	NUMBER_OF_LAYERS
};

namespace resource{

enum class SpriteSheet{
	spriteSheetExample,
	spriteSheetEample2,
	spriteSheetExample3,
	levelsTileset,
	roughSpriteSheet,
	irregularSpriteSheet
};

enum class Font{
	arial
};

enum class Animation{
	walkRough
};

enum class TileMap{
	example
};

enum class Sound{
	punch1,
	punch2
};

}

#endif