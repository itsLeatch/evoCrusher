#pragma once
#include <iostream>
class Bot{
private:
    bool alive = true;
public:
    Bot(){

    }

    void setAlive(const bool& state){
        alive = state;
    }

    bool isAlive(){
        return alive;
    }

    virtual float calculateFitness(){
        return 0.0f;
    }

};