#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class PolygonShape
{
private:
	b2BodyId bodyId;

public:
	PolygonShape() = default;
	PolygonShape(const b2WorldId &worldID, const bool &isDynamic)
	{
		createBody(worldID, isDynamic);
	}

	void createBody(const b2WorldId &worldID, const bool &isDynamic = true);

	void createShape(const b2Polygon &polygonMesh, const b2ShapeDef &shapeDef)
	{
		b2CreatePolygonShape(bodyId, &shapeDef, &polygonMesh);
	}

	b2BodyId getBodyId() const
	{
		return bodyId;
	}

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition();

	void draw(sf::RenderWindow &window);
};