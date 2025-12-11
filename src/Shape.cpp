#include "Shape.h"

void Shape::createBody(const b2WorldId &worldID, const bool &isDynamic)
{
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	if (isDynamic == true)
	{

		groundBodyDef.type = b2_dynamicBody;
	}
	bodyId = b2CreateBody(worldID, &groundBodyDef);
}

bool Shape::containsShape(const b2ShapeId &shape)
{
	size_t shapeCount = b2Body_GetShapeCount(bodyId);
	b2ShapeId *shapes = new b2ShapeId[shapeCount];
	b2Body_GetShapes(bodyId, shapes, shapeCount);
	for (size_t i = 0; i < shapeCount; ++i)
	{
		if (shapes[i].index1 == shape.index1 &&
			shapes[i].world0 == shape.world0) //TODO: check if generation is also needed
		{
			delete[] shapes;
			return true;
		}

		delete[] shapes;
	}
	return false;
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