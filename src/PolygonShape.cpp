#include "PolygonShape.h"

void PolygonShape::createBody(const b2WorldId &worldID, const bool &isDynamic)
{
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	if (isDynamic == true)
	{

		groundBodyDef.type = b2_dynamicBody;
	}
	bodyId = b2CreateBody(worldID, &groundBodyDef); 
}

void PolygonShape::setPosition(const sf::Vector2f &pos)
{
	b2Body_SetTransform(bodyId, b2Vec2(pos.x, pos.y), b2Body_GetRotation(bodyId));
}

sf::Vector2f PolygonShape::getPosition()
{
	b2Vec2 position = b2Body_GetPosition(bodyId);
	return sf::Vector2f(position.x, position.y);
}

void PolygonShape::draw(sf::RenderWindow &window)
{
	std::vector<b2ShapeId> shapeIds;
	shapeIds.resize(b2Body_GetShapeCount(bodyId));
	b2Body_GetShapes(bodyId, shapeIds.data(), shapeIds.size());
	std::vector<sf::Vertex> vertices;
	
	sf::Transform transform;
	sf::Vector2f originalPos;
	b2Vec2 bodyPos = b2Body_GetPosition(bodyId);
	originalPos = sf::Vector2f(bodyPos.x, bodyPos.y);
	transform.rotate(sf::radians(b2Rot_GetAngle(b2Body_GetRotation(bodyId))), originalPos);
	transform.translate(sf::Vector2f(bodyPos.x, bodyPos.y));
	for (const b2ShapeId &shapeId : shapeIds)
	{
		b2Polygon polygonMesh;
		polygonMesh = b2Shape_GetPolygon(shapeId);

		for (uint32_t i = 0; i < polygonMesh.count; i++)
		{
			sf::Vertex vertex;
			vertex.color = sf::Color::White;

			sf::Vector2f localPos = sf::Vector2f(polygonMesh.vertices[i].x, polygonMesh.vertices[i].y);
			vertex.position = transform.transformPoint(localPos);

			vertices.push_back(vertex);
		}
		// Close the loop
			sf::Vertex vertex;
			vertex.color = sf::Color::White;

			sf::Vector2f localPos = sf::Vector2f(polygonMesh.vertices[0].x, polygonMesh.vertices[0].y);
			vertex.position = transform.transformPoint(localPos);

			vertices.push_back(vertex);
	}
	window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::LineStrip);
}
