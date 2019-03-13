/*  MAIN MENU h

### PURPOSE ###
-Defines main menu class interface
-Uses tgui

*/


#ifndef MAIN_MENU_H_GUARD
#define MAIN_MENU_H_GUARD

#include "scene.h"
#include <TGUI/TGUI.hpp>

class MainMenu: public Scene{
public:
	MainMenu(sf::RenderWindow&,const Resources&);
	
	virtual void handleEvent(sf::Event);
	virtual void update(float);
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const;
	
protected:
	
	//Mutable since the tgui::Gui.draw is non-const
	mutable tgui::Gui gui;

};

#endif