#pragma once
#include <torch/torch.h>
#include <box2d/box2d.h>
#include "CircleShape.h"

class Bot : public CircleShape
{
    private:
    bool targetReached = false;
    float invertedFitness = 0.0f;

    torch::Tensor weights = torch::rand({2, 2});

    //take a direction vector with x and y between -1 and 1 and return a velocity vector
    torch::Tensor forewardPass(const torch::Tensor& input){
        //simple neural network with no hidden layers
        torch::Tensor output = torch::matmul(input, weights);
        return output;
    }

    public:
    Bot(){

    }

    void updateDirection(const b2Vec2& targetPos){
        b2Vec2 direction = targetPos - b2Body_GetPosition(bodyId);
        torch::Tensor input = torch::tensor({direction.x, direction.y});
        torch::Tensor output = forewardPass(input);
        b2Vec2 velocity = b2Vec2(output[0].item<float>(), output[1].item<float>());
        velocity = b2Normalize(velocity);
        setVelocity(sf::Vector2f(velocity.x, velocity.y));
    }

    //check if bot reached target
    bool checkAlive(){
        return !targetReached;
    }

    void setTargetReached(const float& time){
        targetReached = true;
        invertedFitness = time; //
    }

    float getInvertedFitness(){
        return invertedFitness;
    }

    //get weights
    torch::Tensor getWeights(){
        return weights;
    }

};