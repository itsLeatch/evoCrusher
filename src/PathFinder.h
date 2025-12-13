#pragma once
#include <torch/torch.h>
#include <box2d/box2d.h>
#include "CircleShape.h"
#include <vector>


struct SensorData{
    float lenght;
    float sensorAngle;
    uint64_t targetType;
    b2RayResult rayResult;
};

class PathFinder : public CircleShape{
private:
    float timeToTarget = 0.0f;
    std::vector<SensorData> sensors;

    torch::Tensor weights = torch::rand({2, 2});

    // take a direction vector with x and y between -1 and 1 and return a velocity vector
    torch::Tensor forewardPass(const torch::Tensor &input)
    {
        // simple neural network with no hidden layers
        torch::Tensor output = torch::matmul(input, weights);
        return output;
    }

public:
    PathFinder() = delete;
    PathFinder(const b2WorldId &worldID, const bool &isDynamic, const std::vector<SensorData>& sensors) : CircleShape(worldID, isDynamic, SHAPE_TYPE_BOT), sensors(sensors)
    {   
        
    }

    void decitionMaking(const b2WorldId& worldID, const b2Vec2 &targetPos)
    {
            for (SensorData& sensor : sensors){
                // update sensor data
                b2Vec2 start = b2Body_GetPosition(bodyId);
                b2Vec2 end = b2Body_GetLinearVelocity(bodyId);
                end = b2RotateVector(b2MakeRot(sensor.sensorAngle), end);
                end = b2Normalize(end) * sensor.lenght;
                sensor.rayResult = b2World_CastRayClosest(worldID, start, end, b2DefaultQueryFilter());
                sensor.targetType = sensor.rayResult.hit ? getShapeTypeFromShapeId(sensor.rayResult.shapeId) : SHAPE_TYPE_NONE;
                if(sensor.rayResult.hit){
                    std::cout << "Sensor hit shape of type: " << sensor.targetType << std::endl;
                }
            }


            b2Vec2 direction = targetPos - b2Body_GetPosition(bodyId);
            torch::Tensor input = torch::tensor({direction.x, direction.y});
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

    virtual void draw(sf::RenderWindow &window) override{
        CircleShape::draw(window);
        // draw sensors
        for(const SensorData& sensor : sensors){
            b2Vec2 start = b2Body_GetPosition(bodyId);
            b2Vec2 end = b2Body_GetLinearVelocity(bodyId);
            if(sensor.rayResult.hit){
                end = sensor.rayResult.point;
            }else{
                end = b2RotateVector(b2MakeRot(sensor.sensorAngle), end);
                end = start + b2Normalize(end) * sensor.lenght;
            }
            
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(start.x, start.y), sf::Color::Red),
                sf::Vertex(sf::Vector2f(end.x, end.y), sf::Color::Red)
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
}; 