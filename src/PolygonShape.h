#pragma once
#include "Shape.h"

class PolygonShape : public Shape
{
private:
	b2BodyId bodyId;

public:
	PolygonShape() = default;
	PolygonShape(const b2WorldId &worldID, const bool &isDynamic)
	{
		createBody(worldID, isDynamic);
		bodyId = getBodyId();
	}

	void createShape(const b2Polygon &polygonMesh, const b2ShapeDef &shapeDef)
	{
		bodyId = getBodyId();
		b2CreatePolygonShape(bodyId, &shapeDef, &polygonMesh);
	}

	void draw(sf::RenderWindow &window) override;
};