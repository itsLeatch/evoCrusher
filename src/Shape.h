#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
class Shape
{
public:
b2BodyId bodyId;
sf::Color color = sf::Color::White;

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

    bool containsShape(const b2ShapeId& shape);

    //positioning
	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition();

    //velocity
    void setVelocity(const sf::Vector2f &vel)
    {
        b2Body_SetLinearVelocity(bodyId, b2Vec2(vel.x, vel.y));
    }
    sf::Vector2f getVelocity()
    {
        b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
        return sf::Vector2f(velocity.x, velocity.y);
    }

	virtual void draw(sf::RenderWindow &window){
        std::cerr << "You sould overload the draw function to get the object drawn!" << std::endl;
    }
};