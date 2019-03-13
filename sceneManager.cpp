
#include "scene.h"
#include <memory>
#include <string>

class SceneData{
public:
	SceneData(){}
private:
	std::string name;
	std::unique_ptr<Scene> scene;
};

class SceneManager: public sf::Drawable{
public:
	SceneManager(){}
	
	virtual void handleEvent(sf::Event){}
	virtual void update(float);
	virtual void draw(sf::RenderTarget& target,sf::RenderStates states)const;
	
private:
	std::vector<SceneData> scenes;
	
};