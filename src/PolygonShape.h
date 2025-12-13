#pragma once
#include "Shape.h"

class PolygonShape : public Shape
{
public:
	PolygonShape() = delete;
	PolygonShape(const b2WorldId &worldID, const bool &isDynamic, const uint64_t& shapeType) : Shape(worldID, isDynamic, shapeType)
	{
		bodyId = getBodyId();
	}

	void createShape(const b2Polygon &polygonMesh, const b2ShapeDef &shapeDef)
	{
		b2CreatePolygonShape(bodyId, &shapeDef, &polygonMesh);
	}

	void draw(sf::RenderWindow &window) override;
};