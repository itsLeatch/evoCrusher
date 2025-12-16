#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>

#define SHAPE_TYPE_NONE 1
#define SHAPE_TYPE_BOT 2
#define SHAPE_TYPE_FOOD 4
#define SHAPE_TYPE_OBSTACLE 8


class Shape
{
public:
    b2BodyId bodyId;
    sf::Color color = sf::Color::White;
    uint64_t shapeType = SHAPE_TYPE_NONE;

    Shape() = delete;
    Shape(const b2WorldId &worldID, const bool &isDynamic, const uint64_t& shapeType) : shapeType(shapeType)
    {
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        if (isDynamic == true)
        {

            groundBodyDef.type = b2_dynamicBody;
        }
        bodyId = b2CreateBody(worldID, &groundBodyDef);
    }
    ~Shape()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    b2BodyId getBodyId() const
    {
        return bodyId;
    }

    bool containsShape(const b2ShapeId &shape);
    static uint64_t getShapeTypeFromShapeId(const b2ShapeId &shapeId)
    {
        b2Filter filter = b2Shape_GetFilter(shapeId);
        return filter.categoryBits;
    }

    // positioning
    void setPosition(const sf::Vector2f &pos);
    sf::Vector2f getPosition();

    // velocity
    void setVelocity(const sf::Vector2f &vel)
    {
        b2Body_SetLinearVelocity(bodyId, b2Vec2(vel.x, vel.y));
    }
    sf::Vector2f getVelocity()
    {
        b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
        return sf::Vector2f(velocity.x, velocity.y);
    }

    virtual void draw(sf::RenderWindow &window)
    {
        std::cerr << "You sould overload the draw function to get the object drawn!" << std::endl;
    }
};