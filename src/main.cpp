#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"
#include "CircleShape.h"

CircleShape player;
PolygonShape target;

const int frameRate = 60;
sf::RenderWindow window;
float zoomFaktor = 100;

b2WorldId worldId;

void createPlayer()
{
    player.createBody(worldId, true);
    player.setPosition(sf::Vector2f(4.0f, 3.0f));
    b2MotionLocks lock = {false, false, true};
    b2Body_SetMotionLocks(player.bodyId, lock);
    b2Circle circleShape;
    circleShape.center = b2Vec2(0, 0);
    circleShape.radius = 0.25;
    b2ShapeDef circleShapeDef = b2DefaultShapeDef();
    circleShapeDef.density = 1.0f;
    circleShapeDef.enableSensorEvents = true;
    player.createShape(circleShape, circleShapeDef);
}

void createTarget()
{
    target.createBody(worldId, false);
    target.setPosition(sf::Vector2f(1, 1));
    target.color = sf::Color::Green;
    b2Polygon box = b2MakeBox(0.5, 0.5);
    b2ShapeDef boxDef = b2DefaultShapeDef();
    boxDef.density = 1.0f;
    boxDef.isSensor = true;
    boxDef.enableSensorEvents = true;
    target.createShape(box, boxDef);
}

void update()
{
    b2SensorEvents events = b2World_GetSensorEvents(worldId);
    for (int i = 0; i < events.beginCount; i++)
    {
        b2SensorBeginTouchEvent event = events.beginEvents[i];
        if (player.containsShape(event.visitorShapeId) && target.containsShape(event.sensorShapeId))
        {
            target.color = sf::Color::Red;
        }
    }

    for (int i = 0; i < events.endCount; i++)
    {
        b2SensorEndTouchEvent event = events.endEvents[i];
        if (player.containsShape(event.visitorShapeId) && target.containsShape(event.sensorShapeId))
        {
            target.color = sf::Color::Green;
        }
    }
}

int main()
{

    // create world
    b2Vec2 gravity = b2Vec2(0, 0);
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    worldId = b2CreateWorld(&world);

    createPlayer();
    createTarget();

    window.create(sf::VideoMode({800, 600}), PROJECT_NAME);
    window.setFramerateLimit(frameRate);

    window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(window.getSize().x / zoomFaktor, window.getSize().y / zoomFaktor))));

    sf::Vector2f resultingVelocity = sf::Vector2f(0.0f, 0.0f);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
                window.close();
            //resize event
            if (event->is<sf::Event::Resized>())
            {
                window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(event->getIf<sf::Event::Resized>()->size.x / zoomFaktor, event->getIf<sf::Event::Resized>()->size.y / zoomFaktor))));
            }
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                {
                    resultingVelocity.y = -5.0f;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                {
                    resultingVelocity.y = 5.0f;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                {
                    resultingVelocity.x = -5.0f;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                {
                    resultingVelocity.x = 5.0f;
                }
            }
            if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                if (keyReleased->scancode == sf::Keyboard::Scancode::W || keyReleased->scancode == sf::Keyboard::Scancode::S)
                {
                    resultingVelocity.y = 0.0f;
                }
                if (keyReleased->scancode == sf::Keyboard::Scancode::A || keyReleased->scancode == sf::Keyboard::Scancode::D)
                {
                    resultingVelocity.x = 0.0f;
                }
            }
        }
        player.setVelocity(resultingVelocity);

        // clear the window with black color
        window.clear(sf::Color::Black);

        b2World_Step(worldId, 1.0f / frameRate, 4);
        update();

        // b2World_Draw(worldId, &debugDraw);
        player.draw(window);
        target.draw(window);

        // end the current frame
        window.display();
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}