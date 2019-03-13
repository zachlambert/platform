/* MAIN MENU cpp

### PURPOSE ### 
-Defines main menu class implementation

*/


#include "mainMenu.h"
#include "simpleScene.h"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& window,const Resources& resources):Scene(window,resources),gui(window){
	tgui::Button::Ptr button = tgui::Button::create();
	button->setSize(200,50);
	button->setPosition(window.getSize().x/2-100,window.getSize().y/2-25);
	button->setText("Start Game");
	button->connect("pressed",changeSceneFunctor(this,new SimpleScene(window,resources)));

	gui.add(button,"mainButton");
}

void MainMenu::handleEvent(sf::Event event){
	gui.handleEvent(event);
}

void MainMenu::update(float seconds){
	
}
void MainMenu::draw(sf::RenderTarget& target,sf::RenderStates states)const{
	gui.draw();
}