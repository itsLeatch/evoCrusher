#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"

PolygonShape beam;
PolygonShape ground;

const int frameRate = 60;
int main()
{
    // create world
    b2Vec2 gravity(0, 2.0f);
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    b2WorldId worldId = b2CreateWorld(&world);

    beam.createBody(worldId, true);
    beam.setPosition(sf::Vector2f(5.0f, 2.0f));

    b2Polygon groundBox = b2MakeBox(4.5f, .5f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.density = 1.0f;
    beam.createShape(groundBox, groundShapeDef);

    ground.createBody(worldId, false);
    ground.setPosition(sf::Vector2f(2.0f, 4.0f));

    b2Polygon secondBox = b2MakeBox(.5f, .5f);
    b2ShapeDef secondShapeDef = b2DefaultShapeDef();
    secondShapeDef.density = 1.0f;
    ground.createShape(secondBox, secondShapeDef);

    sf::RenderWindow window(sf::VideoMode({800, 600}), PROJECT_NAME);
    window.setFramerateLimit(frameRate);

    window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(10, 10.f))));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        b2World_Step(worldId, 1.0f / frameRate, 4);

        beam.draw(window);
        ground.draw(window);

        // end the current frame
        window.display();
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}