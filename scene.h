/* SCENE h

### PURPOSE ###
-Contains the scene class
-Defines a functor for changing class (overloads ()), which when called, calls changeScene on a class
	Used when the changeScene function can't be called directly


### SCENE CLASS INTERFACE ###

Constructor(window,resources)
	Stores the window and resources objects, as references.
	Initialises the view, using the window dimensions
	Initialises the new scene pointer, to null (0)

handleEvent(event)
	Provided an event, which is handled.
	Virtual function. Child class defines how events are handled.

update(seconds)
	Updates the game logic, using time since last update.
	Virtual function. Child class defines what is updated each cycle.

draw(render target, render states)
	Draws the scene on the target, using the given render states.
	Virtual function. Child class defines how the scene is drawn.

getView()
	Return the view object, which is used to define which area of the scene is displayed on the window.

getNewScene()
	Return the new scene pointer. Checked in the game loop, and scene changed if this pointer is set.

changeScene(new scene)
	Loads a new scene to load, which will replace the current scene. The game loop updates the current scene.

*/


#ifndef SCENE_H_GUARD
#define SCENE_H_GUARD

#include <SFML/Graphics.hpp>
#include "resources.h"

struct changeSceneFunctor;

class Scene: public sf::Drawable{

public:
	Scene(sf::RenderWindow& window,const Resources& resources):window(window),resources(resources),
		view(sf::FloatRect(0,0,window.getSize().x,window.getSize().y)),newScene(0){}
	
	virtual void handleEvent(sf::Event event){}
	virtual void update(float seconds)=0;
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const=0; //From sf::Drawable
	
	const sf::View& getView()const{ return view; }
	Scene* getNewScene()const{return newScene;}
	void changeScene(Scene* newScene) { this->newScene = newScene; }

protected:
	sf::RenderWindow& window;
	const Resources& resources;
	sf::View view;
private:
	Scene* newScene;
};

struct changeSceneFunctor{
	changeSceneFunctor(Scene* scene,Scene* newScene):scene(scene),newScene(newScene){}
	void operator()(){scene->changeScene(newScene);}
private:
	Scene* scene;
	Scene* newScene;
};

#endif