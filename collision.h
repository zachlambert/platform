#ifndef COLLISION_H_GUARD
#define COLLISION_H_GUARD

#include <SFML/Graphics.hpp>
#include "hitbox.h"
#include "tileMap.h"

const bool collide(const sf::Transformable&, Hitbox&, const sf::Transformable&, Hitbox&);
const bool collideTileMap(const TileMap&, const sf::Transformable&, Hitbox&);

#endif
