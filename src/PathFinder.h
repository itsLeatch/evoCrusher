#pragma once
#include <torch/torch.h>
#include <box2d/box2d.h>
#include "CircleShape.h"
#include <vector>

struct SensorData
{
    float lenght;
    float sensorAngle;
    uint64_t targetType;
    b2RayResult rayResult;
};

class PathFinder : public CircleShape
{
private:
    float footCollected = 0.0f;

private:
    std::vector<SensorData> sensors;
    // weights random between -1 and 1
    torch::Tensor weights = torch::rand({10, 2}) * 2 - 1;

    // take a direction vector with x and y between -1 and 1 and return a velocity vector
    torch::Tensor forewardPass(const torch::Tensor &input)
    {
        // simple neural network with no hidden layers
        torch::Tensor output = torch::matmul(input, weights);
        return output;
    }

public:
    PathFinder() = delete;
    PathFinder(const b2WorldId &worldID, const bool &isDynamic, const std::vector<SensorData> &sensors) : CircleShape(worldID, isDynamic, SHAPE_TYPE_BOT), sensors(sensors)
    {
    }

    void decitionMaking()
    {
        for (SensorData &sensor : sensors)
        {
            // update sensor data
            b2Vec2 start = b2Body_GetPosition(bodyId);
            b2Vec2 end = b2Body_GetLinearVelocity(bodyId);
            end = b2RotateVector(b2MakeRot(sensor.sensorAngle), end);
            end = b2Normalize(end) * sensor.lenght;
            b2QueryFilter filter = b2DefaultQueryFilter();
            b2ShapeId bodyShapes;
            b2Body_GetShapes(bodyId, &bodyShapes, 1);
            filter.maskBits = b2Shape_GetFilter(bodyShapes).maskBits;
            sensor.rayResult = b2World_CastRayClosest(b2Body_GetWorld(bodyId), start, end, filter);
            sensor.targetType = sensor.rayResult.hit ? getShapeTypeFromShapeId(sensor.rayResult.shapeId) : SHAPE_TYPE_NONE;
        }
        torch::Tensor input = torch::tensor({});
        // create input vector
        // add for each sensor the distance to the hit point, hit is food
        for (const SensorData &sensor : sensors)
        {
            float distance = sensor.rayResult.hit ? b2Distance(b2Body_GetPosition(bodyId), sensor.rayResult.point) / sensor.lenght : sensor.lenght;
            float isTarget = (sensor.targetType == SHAPE_TYPE_FOOD) ? 1.0f : 0.0f;
            float isObstacle = (sensor.targetType == SHAPE_TYPE_OBSTACLE) ? 1.0f : 0.0f;
            input = torch::cat({input, torch::tensor({distance, isTarget, isObstacle})});
        }
        // add a bias input
        input = torch::cat({input, torch::tensor({1.0f})});

        torch::Tensor output = forewardPass(input);
        b2Vec2 velocity = b2Vec2(output[0].item<float>(), output[1].item<float>());
        velocity = b2Normalize(velocity);
        setVelocity(sf::Vector2f(velocity.x, velocity.y));
    }

    // get weights
    torch::Tensor getWeights()
    {
        return weights;
    }

    void setWeights(const torch::Tensor &newWeights)
    {
        weights = newWeights;
    }

    float getFootCollected() const
    {
        return footCollected;
    }

    void collectFoot()
    {
        footCollected += 1.0f;
    }

    virtual void draw(sf::RenderWindow &window) override
    {
        CircleShape::draw(window);
        // draw foot collected as text above the bot
        //TODO: use a standard font and not load it every frame
        sf::Font font;
        if (!font.openFromFile("./assets/arial.ttf"))
        {
            std::cerr << "Failed to load font!" << std::endl;
            return;
        }

        sf::Text text(font);
        text.setString(std::to_string(static_cast<int>(footCollected)));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::Green);
        sf::Vector2f position = getPosition();
        sf::Vector2i globalPos = window.mapCoordsToPixel(position);
        globalPos.y -= 32; 
        auto recoveryView = window.getView();
        window.setView(window.getDefaultView());
        text.setPosition(sf::Vector2f(globalPos.x, globalPos.y));
        window.draw(text);
        window.setView(recoveryView);

        // draw sensors
        for (const SensorData &sensor : sensors)
        {
            b2Vec2 start = b2Body_GetPosition(bodyId);
            b2Vec2 end = b2Body_GetLinearVelocity(bodyId);
            if (sensor.rayResult.hit)
            {
                end = sensor.rayResult.point;
            }
            else
            {
                end = b2RotateVector(b2MakeRot(sensor.sensorAngle), end);
                end = start + b2Normalize(end) * sensor.lenght;
            }

            sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(start.x, start.y), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(end.x, end.y), sf::Color::Red)};
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
};