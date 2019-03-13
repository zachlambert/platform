#ifndef MUSIC_H_GUARD
#define MUSIC_H_GUARD

#include <SFML/Audio/Music.hpp>
#include <string>

const bool loadMusic(sf::Music&,const std::string&);
void playMusic(sf::Music&,const std::string&);

#endif