#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <iostream>
#include <vector>

#include "cmakeInput.h"
#include "PolygonShape.h"
#include "CircleShape.h"

CircleShape ball;
PolygonShape ground;

const int frameRate = 60;
sf::RenderWindow window;
/*
void drawSolidPolygons(b2Transform transform, const b2Vec2 *vertices, int vertexCount, float radius, b2HexColor color,
                       void *context)
{
    
	std::vector<sf::Vertex> drawVertices;
	
	sf::Transform vertexTransform;
	sf::Vector2f originPos;

    originPos = sf::Vector2f(transform.p.x, transform.p.y);
	vertexTransform.rotate(sf::radians(b2Rot_GetAngle(transform.q)), originPos);
	vertexTransform.translate(originPos);
	
	for (uint32_t i = 0; i < vertexCount; i++)
		{
			sf::Vertex vertex;
			vertex.color = sf::Color(color);

			sf::Vector2f localPos = sf::Vector2f(vertices[i].x, vertices[i].y);
			vertex.position = vertexTransform.transformPoint(localPos);

			drawVertices.push_back(vertex);
		}
        //close the loop
			drawVertices.push_back(drawVertices[0]);
	
	window.draw(drawVertices.data(), drawVertices.size(), sf::PrimitiveType::LineStrip);
}*/

int main()
{

    // create world
    b2Vec2 gravity(0, 9.0f);
    b2WorldDef world = b2DefaultWorldDef();
    world.gravity = gravity;
    b2WorldId worldId = b2CreateWorld(&world);

    /*b2DebugDraw debugDraw = b2DefaultDebugDraw();
    debugDraw.DrawSolidPolygonFcn = drawSolidPolygons;
    debugDraw.drawShapes = true;*/

    ball.createBody(worldId, true);
    ball.setPosition(sf::Vector2f(5.0f, 0.0f));

    b2Circle circleShape;
    circleShape.center = b2Vec2(0,0);
    circleShape.radius = 1;
    b2ShapeDef circleShapeDef = b2DefaultShapeDef();
    circleShapeDef.density = 1.0f;
    b2SurfaceMaterial material = b2DefaultSurfaceMaterial();
    material.restitution = 0.8;
    circleShapeDef.material = material; 
    ball.createShape(circleShape, circleShapeDef);

    ground.createBody(worldId, false);
    ground.setPosition(sf::Vector2f(2.0f, 4.0f));

    b2Polygon secondBox = b2MakeBox(3.5f, .5f);
    b2Circle cirkel;
    b2ShapeDef secondShapeDef = b2DefaultShapeDef();
    secondShapeDef.density = 1.0f;
    ground.createShape(secondBox, secondShapeDef);

    window.create(sf::VideoMode({800, 600}), PROJECT_NAME);
    window.setFramerateLimit(frameRate);

    window.setView(sf::View(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(8, 6))));

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

        //b2World_Draw(worldId, &debugDraw);
        ball.draw(window);
        ground.draw(window);

        // end the current frame
        window.display();
    }

    b2DestroyWorld(worldId);
    worldId = b2_nullWorldId;
}