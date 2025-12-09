#include "CircleShape.h"

void CircleShape::setRadius(const float &radius)
{
    // TODO: make sure, that the first stored object is the circle
    b2ShapeId circleShapeID;
    b2Body_GetShapes(getBodyId(), &circleShapeID, 1);
    b2Circle newCircleShape;
    newCircleShape.center = b2Vec2(newCircleShape.center.x, newCircleShape.center.y);
    newCircleShape.radius = radius;
    b2Shape_SetCircle(circleShapeID,&newCircleShape);
}
float CircleShape::getRadius()
{
    // TODO: make sure, that the first stored object is the circle
    b2ShapeId circleShapeID;
    b2Body_GetShapes(getBodyId(), &circleShapeID, 1);
    auto circleShape = b2Shape_GetCircle(circleShapeID);
    return circleShape.radius;
}

void CircleShape::draw(sf::RenderWindow &window)
{
    sf::CircleShape sprite;
    sprite.setPosition(getPosition());
    sprite.setRadius(getRadius());
    sf::Vector2f origin;
    origin.x = sprite.getRadius();
    origin.y = origin.x;
    sprite.setOrigin(origin);

    window.draw(sprite);
}