#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "Shape.h"

class CircleShape : public Shape
{
private:

public:
	CircleShape() = default;
	CircleShape(const b2WorldId &worldID, const bool &isDynamic)
	{
		createBody(worldID, isDynamic);
	}

	void createShape(const b2Circle &circleMesh, const b2ShapeDef &shapeDef)
	{
		b2CreateCircleShape(bodyId, &shapeDef, &circleMesh);
	}

    void setRadius(const float& radius);
    float getRadius();

	void draw(sf::RenderWindow &window) override;
};