#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "Shape.h"

class CircleShape : public Shape
{
private:

public:
	CircleShape() = delete;
	CircleShape(const b2WorldId &worldID, const bool &isDynamic, const uint64_t& shapeType) : Shape(worldID, isDynamic, shapeType)
	{
		
	}

	void createShape(const b2Circle &circleMesh, b2ShapeDef &shapeDef)
	{
		shapeDef.filter.categoryBits = shapeType;
		b2CreateCircleShape(bodyId, &shapeDef, &circleMesh);
	}

    void setRadius(const float& radius);
    float getRadius();

	virtual void draw(sf::RenderWindow &window) override;
};