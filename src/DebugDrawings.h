#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>


class DebugDrawings : public b2DebugDraw
{
private:
    sf::RenderTarget *target;

    void initFunctions()
    {
        //TODO: make font loading configurable
        font.openFromFile("./assets/arial.ttf");

        this->DrawSolidPolygonFcn = drawSolidPolygon;
        this->DrawSolidCircleFcn = drawSolidCircle;
        this->DrawSegmentFcn = drawLine;
        this->DrawPointFcn = drawPoint;
        this->DrawStringFcn = drawString;
    }

public:
    static sf::Font font;

    DebugDrawings() : b2DebugDraw(b2DefaultDebugDraw())
    {
        initFunctions();
    }

    DebugDrawings(sf::RenderTarget &target) : b2DebugDraw(b2DefaultDebugDraw())
    {
        setRenderTarget(target);
        initFunctions();
    }

    void setRenderTarget(sf::RenderTarget &target)
    {
        this->target = &target;
        context = static_cast<void*>(&target);
    }

    sf::RenderTarget *getRenderTarget()
    {
        return target;
    }

    static void drawSFPolygons( b2Transform transform, const b2Vec2* vertices, int vertexCount, b2HexColor color,
                                void* context ){
                                    
                                }

     static void drawSolidPolygon( b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color,
								void* context ){
        sf::RenderTarget* target = static_cast<sf::RenderTarget*>(context);
        sf::ConvexShape polygon(vertexCount);
        polygon.setOrigin(sf::Vector2f(0.0f, 0.0f));
        polygon.setPosition(sf::Vector2f(transform.p.x, transform.p.y));
        float angle = b2Rot_GetAngle(transform.q);
        sf::Angle angleDeg = sf::degrees(angle);
        polygon.setRotation(angleDeg);

        for (int i = 0; i < vertexCount; i++)
        {
            polygon.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
        }
        polygon.setFillColor(sf::Color(color));
        if (target != nullptr)
        {
            target->draw(polygon);
        }
        else
        {
            std::cout << "No render target set for DebugDraw!" << std::endl;
        }
    }

    static void drawSolidCircle( b2Transform transform, float radius, b2HexColor color, void* context ){
        sf::RenderTarget* target = static_cast<sf::RenderTarget*>(context);
        sf::CircleShape circle(radius);
        circle.setOrigin(sf::Vector2f(radius, radius));
        circle.setPosition(sf::Vector2f(transform.p.x, transform.p.y));
        float angle = b2Rot_GetAngle(transform.q);
        sf::Angle angleDeg = sf::degrees(angle);
        circle.setRotation(angleDeg);
        circle.setFillColor(sf::Color(color));
        if (target != nullptr)
        {
            target->draw(circle);
        }
        else
        {
            std::cout << "No render target set for DebugDraw!" << std::endl;
        }
    }

    static void drawLine( b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context ){
        sf::RenderTarget* target = static_cast<sf::RenderTarget*>(context);
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(p1.x, p1.y), sf::Color(color)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y), sf::Color(color))
        };
        if (target != nullptr)
        {
            target->draw(line, 2, sf::PrimitiveType::Lines);
        }
        else
        {
            std::cout << "No render target set for DebugDraw!" << std::endl;
        }
    }

    static void drawPoint( b2Vec2 p, float size, b2HexColor color, void* context ){
        sf::RenderTarget* target = static_cast<sf::RenderTarget*>(context);
        sf::CircleShape point(size);
        point.setOrigin(sf::Vector2f(size, size));
        point.setPosition(sf::Vector2f(p.x, p.y));
        point.setFillColor(sf::Color(color));
        if (target != nullptr)
        {
            target->draw(point);
        }
        else
        {
            std::cout << "No render target set for DebugDraw!" << std::endl;
        }
    }

    static void drawString( b2Vec2 p, const char* s, b2HexColor color, void* context ){
        sf::RenderTarget* target = static_cast<sf::RenderTarget*>(context);
        sf::Text text(font);
        text.setString(s);
        text.setCharacterSize(12);
        text.setPosition(sf::Vector2f(p.x, p.y));
        text.setFillColor(sf::Color(color));
        if (target != nullptr)
        {
            target->draw(text);
            return;
        }
        else
        {
            std::cout << "No render target set for DebugDraw!" << std::endl;
        }
    
    }
};

sf::Font DebugDrawings::font;