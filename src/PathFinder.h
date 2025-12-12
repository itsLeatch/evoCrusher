#pragma once
#include <torch/torch.h>
#include <box2d/box2d.h>
#include "CircleShape.h"

class PathFinder : public CircleShape{
private:
    float timeToTarget = 0.0f;

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
    PathFinder(const b2WorldId &worldID, const bool &isDynamic) : CircleShape(worldID, isDynamic)
    {
        
    }

    void updateDirection(const b2Vec2 &targetPos)
    {


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
};