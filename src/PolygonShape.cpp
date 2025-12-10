#include "PolygonShape.h"


void PolygonShape::draw(sf::RenderWindow &window)
{
	std::vector<b2ShapeId> shapeIds;
	shapeIds.resize(b2Body_GetShapeCount(bodyId));
	b2Body_GetShapes(bodyId, shapeIds.data(), shapeIds.size());
	std::vector<sf::Vertex> vertices;
	
	sf::Transform transform;
	sf::Vector2f origin = getPosition();
	transform.rotate(sf::radians(b2Rot_GetAngle(b2Body_GetRotation(bodyId))), origin);
	transform.translate(origin);
	for (const b2ShapeId &shapeId : shapeIds)
	{
		b2Polygon polygonMesh;
		polygonMesh = b2Shape_GetPolygon(shapeId);

		for (uint32_t i = 0; i < polygonMesh.count; i++)
		{
			sf::Vertex vertex;
			vertex.color = color;

			sf::Vector2f localPos = sf::Vector2f(polygonMesh.vertices[i].x, polygonMesh.vertices[i].y);
			vertex.position = transform.transformPoint(localPos);

			vertices.push_back(vertex);
		}
		// Close the loop
			vertices.push_back(vertices[0]);
	}
	window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::LineStrip);
}
