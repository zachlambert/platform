/* MAIN cpp

### PURPOSE ###
-Loads resources, and stores in an object
-Initialises window, which will be drawn to
-Initialises starting scene
-Executes the game loop, which has 4 parts:
	-Event handling: Polls events, and passes to current scene
	-Update: Updates current scene with time elapsed since last update
	-Draw: Draws current scene
	-Check for scene change: Checks scene->getNewScene, and changes the scene if a new scene is provided

*/

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "resources.h"
#include "scene.h"
#include "mainMenu.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;
const int FPS_LIMIT = 120;
const std::string WINDOW_TITLE("Game");

int main(){
	
	std::cout<<"Starting"<<std::endl;

	//Load resources
	Resources resources;
	if(!loadResources(resources)){
		std::cout<<"Failed to load resources."<<std::endl;
		return 1;
	}
	
	//Create window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT),WINDOW_TITLE,sf::Style::Close);
	window.setKeyRepeatEnabled(false);
	
	//Load starting scene
	std::unique_ptr<Scene> scene = std::unique_ptr<Scene>(new MainMenu(window,resources));
	
	//Set the window view
	window.setView(scene->getView());
	
	sf::Clock clock;
	sf::Time elapsed;
	float seconds = 0;
	
	//*** Game Loop ***

	while(window.isOpen()){

		elapsed = clock.restart();
		seconds += elapsed.asSeconds();
		
		if(seconds>1.f/FPS_LIMIT){ //If I don't have a max fps, sometimes seconds is too low and causes program to stop responding for some reason.
			
			//*** Event Handling ***

			sf::Event event;
			while(window.pollEvent(event)){
				if(event.type == sf::Event::Closed){
					window.close();
				}
				scene->handleEvent(event);
			}
			
			//*** Update Scene ***

			scene->update(seconds);
			
			//*** Draw Scene ***

			window.setView(scene->getView());
			window.clear(sf::Color::White);
			window.draw(*scene);
			window.display();
			
			//*** Check for Scene Change ***

			if(scene->getNewScene()){
				Scene* newScene = scene->getNewScene();
				scene.reset(newScene);
			}

			seconds=0;
		}
		
	}
	return 0;
}