// #include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"
#include "CircleShape.h"
#include "PathFinder.h"
#include "DebugDrawings.h"

#include <torch/torch.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <random>
#include <algorithm>
#include <memory>

std::random_device dev;
std::mt19937 rng(dev());

DebugDrawings debugDrawings;

std::vector<std::shared_ptr<PathFinder>> players;
std::shared_ptr<PolygonShape> target;

const int frameRate = 60;
const float timeStepsPerSecond = 240.0f;

float speed = 1.0f;

sf::RenderWindow window;
float zoomFaktor = 100;
b2WorldId worldId;

sf::View camera = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800 / zoomFaktor, 600 / zoomFaktor));

sf::Time maxTimePerGeneration = sf::seconds(7);
size_t worldSteps = 0;
size_t generationNumber = 0;

float getTimeSinceWorldStart()
{
    return worldSteps / timeStepsPerSecond;
}

void generateBotGeneration()
{
    players.clear();
    for (size_t i = 0; i < 10; i++)
    {
            std::vector<SensorData> sensors = {
        SensorData(1.0f, 0.0f, SHAPE_TYPE_NONE, b2RayResult()),
        SensorData(1.0f, -B2_PI / 4.0f, SHAPE_TYPE_NONE, b2RayResult()),
        SensorData(1.0f, B2_PI / 4.0f, SHAPE_TYPE_NONE, b2RayResult()),
    };
        players.push_back(std::make_unique<PathFinder>(worldId, true, sensors));
        std::shared_ptr<PathFinder> player = players[players.size() - 1];
        player->setPosition(sf::Vector2f(0.0f, 0.0f));
        b2Body_SetFixedRotation(player->bodyId, true);
        b2Circle circleShape;
        circleShape.center = b2Vec2(0, 0);
        circleShape.radius = 0.1;
        b2ShapeDef circleShapeDef = b2DefaultShapeDef();
        // colide with everything except other bots
        circleShapeDef.filter.maskBits = 0xFFFFFFFF & ~0x00000002;
        circleShapeDef.density = 1.0f;
        circleShapeDef.enableSensorEvents = true;
        player->createShape(circleShape, circleShapeDef);

    }
}

void createTarget()
{
    target.reset();
    target = std::make_shared<PolygonShape>(worldId, false, SHAPE_TYPE_FOOD);
    std::uniform_int_distribution<std::mt19937::result_type> randomPos(0, 7);
    target->setPosition(sf::Vector2f(randomPos(rng) - 3.5, randomPos(rng) - 3.5));
    target->color = sf::Color::Green;
    b2Polygon box = b2MakeBox(1, 1);
    b2ShapeDef boxDef = b2DefaultShapeDef();
    boxDef.density = 1.0f;
    boxDef.isSensor = true;
    boxDef.enableSensorEvents = true;
    target->createShape(box, boxDef);
}

void generateWorld()
{
    // reset world
    if (worldId.index1 != b2_nullWorldId.index1)
    {
        players.clear();
        b2DestroyWorld(worldId);
    }
    worldId = b2_nullWorldId;

    worldSteps = 0;

    // create world
    b2Vec2 gravity = b2Vec2(0, 0);
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    worldId = b2CreateWorld(&world);

    generateBotGeneration();
    createTarget();
}

void updateBox2dEvents()
{
    
    b2SensorEvents events = b2World_GetSensorEvents(worldId);
    for (int i = 0; i < events.beginCount; i++)
    {
        b2SensorBeginTouchEvent event = events.beginEvents[i];

        // use one of the build in algorithms to remove bots that reached the target
        for (size_t i = 0; i < players.size(); ++i)
        {
            if (players[i]->containsShape(event.visitorShapeId) && target->containsShape(event.sensorShapeId))
            {
                players.erase(std::begin(players) + i);
                --i; 
                break;
            }
        }
    }
}

void startNewGeneration()
{
    generationNumber++;
    generateWorld();
}

void updateBotStates()
{

    for (auto &player : players)
        player->decitionMaking(worldId, b2Body_GetPosition(target->bodyId));
}

void init()
{
    startNewGeneration();

    window.create(sf::VideoMode({800, 600}), PROJECT_NAME);
    window.setFramerateLimit(frameRate);
    window.setView(camera);
    debugDrawings.setRenderTarget(window);
    debugDrawings.drawContactFeatures = true;
    ImGui::SFML::Init(window);
}

void draw()
{
    for (auto &player : players)
    {
        player->draw(window);
    }
    target->draw(window);
}

int main()
{
    init();

    sf::Clock deltaClock;
    sf::Vector2f resultingVelocity = sf::Vector2f(0.0f, 0.0f);
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
                camera.setSize(sf::Vector2f(resizeEvent->size.x / zoomFaktor, resizeEvent->size.y / zoomFaktor));
                window.setView(camera);
            }
        }
        // clear the window with black color
        window.clear(sf::Color::Black);

        ImGui::SFML::Update(window, deltaClock.restart());
        updateBotStates();
        size_t worldstepsInThisFrame = static_cast<size_t>(timeStepsPerSecond / frameRate * speed);
        worldstepsInThisFrame <= 0 ? worldstepsInThisFrame = 1 : worldstepsInThisFrame = worldstepsInThisFrame;
        b2World_Step(worldId, 1.0f * speed / frameRate, worldstepsInThisFrame);
        worldSteps += worldstepsInThisFrame;
        updateBox2dEvents();
        draw();
        b2World_Draw(worldId, &debugDrawings);
        ImGui::Begin("Generation Info");
        ImGui::Text("Generation: %d", generationNumber);
        ImGui::Text("Time: %.2f", getTimeSinceWorldStart());
        ImGui::SliderFloat("Speed", &speed, 0.1f, 50.0f);
        ImGui::End();

        ImGui::SFML::Render(window);
        // end the current frame
        window.display();
        if (getTimeSinceWorldStart() >= maxTimePerGeneration.asSeconds() || players.empty())
        {
            startNewGeneration();
        }
    }
    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}