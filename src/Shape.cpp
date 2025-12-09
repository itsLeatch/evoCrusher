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

void Shape::setPosition(const sf::Vector2f &pos)
{
	b2Body_SetTransform(bodyId, b2Vec2(pos.x, pos.y), b2Body_GetRotation(bodyId));
}

sf::Vector2f Shape::getPosition()
{
	b2Vec2 position = b2Body_GetPosition(bodyId);
	return sf::Vector2f(position.x, position.y);
}
