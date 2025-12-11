// #include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"
#include "CircleShape.h"
#include "Bot.h"

#include <torch/torch.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <random>

Bot players[10];
PolygonShape target;

const int frameRate = 60;
sf::RenderWindow window;
float zoomFaktor = 100;
b2WorldId worldId;

std::random_device dev;
std::mt19937 rng(dev());

sf::View Camera = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800 / zoomFaktor, 600 / zoomFaktor));

sf::Clock timeSinceStart;
void createPlayer()
{
    for (auto &player : players)
    {
        player.createBody(worldId, true);
        player.setPosition(sf::Vector2f(0.0f, 0.0f));
        b2Body_SetFixedRotation(player.bodyId, true);
        b2Circle circleShape;
        circleShape.center = b2Vec2(0, 0);
        circleShape.radius = 0.3;
        b2ShapeDef circleShapeDef = b2DefaultShapeDef();

        circleShapeDef.filter.categoryBits = 0x00000002;
        // colide with everything except other bots
        circleShapeDef.filter.maskBits = 0xFFFFFFFF & ~0x00000002;
        circleShapeDef.density = 1.0f;
        circleShapeDef.enableSensorEvents = true;
        player.createShape(circleShape, circleShapeDef);
    }
}

void createTarget()
{
    target.createBody(worldId, false);
    std::uniform_int_distribution<std::mt19937::result_type> randomPos(0, 7);
    target.setPosition(sf::Vector2f(randomPos(rng) - 3.5, randomPos(rng) - 3.5));
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
        for (auto &player : players)
        {

            if (player.containsShape(event.visitorShapeId) && target.containsShape(event.sensorShapeId))
            {
                target.color = sf::Color::Red;
                player.setTargetReached(timeSinceStart.getElapsedTime().asSeconds());
                std::cout << player.getInvertedFitness() << "weights: " << player.getWeights() << std::endl;
            }
        }
    }
//no end events needed for now
}

void init()
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
    window.setView(Camera);
    ImGui::SFML::Init(window);
}

int main()
{
    init();

    sf::Clock deltaClock;
    sf::Vector2f resultingVelocity = sf::Vector2f(0.0f, 0.0f);
    timeSinceStart.start();
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
            // resize event
            if (event->is<sf::Event::Resized>())
            {
                const auto &resizeEvent = event->getIf<sf::Event::Resized>();
                Camera.setSize(sf::Vector2f(resizeEvent->size.x / zoomFaktor, resizeEvent->size.y / zoomFaktor));
                window.setView(Camera);
            }
        }
        // clear the window with black color
        window.clear(sf::Color::Black);

        ImGui::SFML::Update(window, deltaClock.restart());
        for (auto &player : players)
            player.updateDirection(b2Body_GetPosition(target.bodyId));
        b2World_Step(worldId, 1.0f / frameRate, 4);
        update();
        for (auto &player : players){
            if(player.checkAlive()){
                player.draw(window);
            }
        }
        target.draw(window);
        ImGui::SFML::Render(window);
        // end the current frame
        window.display();
    }
    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}