#include "Shape.h"

bool Shape::containsShape(const b2ShapeId &shape)
{
	auto bodyOfShape = b2Shape_GetBody(shape);
	return bodyOfShape.world0 == bodyId.world0 && bodyOfShape.index1 == bodyId.index1;
}

void Shape::setPosition(const sf::Vector2f &pos)
{
	b2Body_SetTransform(bodyId, b2Vec2(pos.x, pos.y), b2Body_GetRotation(bodyId));
}

sf::Vector2f Shape::getPosition()
{
	b2Vec2 position = b2Body_GetPosition(bodyId);
	return sf::Vector2f(position.x, position.y);
}