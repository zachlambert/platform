#ifndef MOTION_H_GUARD
#define MOTION_H_GUARD

#include <SFML/Graphics.hpp>

class Motion {
public:

	Motion():displacement(0,0), velocity(0, 0), acceleration(0, 0), inFreeFall(false) {}

	void update(float seconds);

	void setVelocity(sf::Vector2f velocity) { this->velocity = velocity; }
	void setVelocity(float vx,float vy){ this->velocity = sf::Vector2f(vx,vy); }
	void setVelocityX(float vx){ velocity.x = vx; }
	void setVelocityY(float vy){ velocity.y = vy; }

	void setAcceleration(sf::Vector2f acceleration) { this->acceleration = acceleration; }
	void setAcceleration(float ax, float ay){ this->acceleration = sf::Vector2f(ax,ay); }
	void setAccelerationX(float ax){ acceleration.x = ax; }
	void setAccelerationY(float ay){ acceleration.y = ay; }

	const sf::Vector2f& getDisplacement()const{ return displacement; }

	void setInFreeFall(bool inFreeFall);
	bool isInFreeFall()const{ return inFreeFall; }
private:
	sf::Vector2f displacement;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	bool inFreeFall;
};

#endif
