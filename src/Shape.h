#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
class Shape
{
private:
	b2BodyId bodyId;

public:
	Shape() = default;
	Shape(const b2WorldId &worldID, const bool &isDynamic)
	{
		createBody(worldID, isDynamic);
	}

	void createBody(const b2WorldId &worldID, const bool &isDynamic = true);

	b2BodyId getBodyId() const
	{
		return bodyId;
	}

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition();

	virtual void draw(sf::RenderWindow &window){
        std::cerr << "You sould overload the draw function to get the object drawn!" << std::endl;
    }
};