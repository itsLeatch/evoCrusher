//#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"
#include "CircleShape.h"

#include <torch/torch.h>
#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

CircleShape player;
PolygonShape target;

const int frameRate = 60;
sf::RenderWindow window;
float zoomFaktor = 100;
b2WorldId worldId;


sf::View Camera = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(800, 600));
void createPlayer()
{
    player.createBody(worldId, true);
    player.setPosition(sf::Vector2f(4.0f, 3.0f));
    b2Body_SetFixedRotation(player.bodyId, true);
    b2Circle circleShape;
    circleShape.center = b2Vec2(0, 0);
    circleShape.radius = 0.3;
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

void torchTest(){
    std::cout << "Torch test" << std::endl;
    torch::Tensor tensor = torch::eye(3);
    std::cout << tensor << std::endl;

    //vector with random entries
    torch::Tensor randTensor = torch::rand({2,3});
    std::cout << randTensor << std::endl;

    torch::Tensor biggerTensor = torch::rand({4,4,5});
    std::cout << biggerTensor << std::endl;
}

int main()
{
    torchTest();
    
    
    //create world
    b2Vec2 gravity = b2Vec2(0, 0);
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    worldId = b2CreateWorld(&world);

    createPlayer();
    createTarget();

    window.create(sf::VideoMode({800,600}), PROJECT_NAME);
    window.setFramerateLimit(frameRate);
    ImGui::SFML::Init(window);


    
    sf::Clock deltaClock;
    sf::Vector2f resultingVelocity = sf::Vector2f(0.0f, 0.0f);
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();
            //resize event
            if (event->is<sf::Event::Resized>())
            {
                const auto& resizeEvent = event->getIf<sf::Event::Resized>();
                Camera.setSize(sf::Vector2f(resizeEvent->size.x / zoomFaktor, resizeEvent->size.y / zoomFaktor));
                window.setView(Camera);
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

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        b2World_Step(worldId, 1.0f / frameRate, 4);
        update();

        player.draw(window);
        target.draw(window);
        ImGui::SFML::Render(window);
        // end the current frame
        window.display();
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}