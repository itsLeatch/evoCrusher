# EvoCrusher
EvoCrusher is a simulation where bots are spawned into an environment to compete for resources and evolve over time. The goal is to create bots that can adapt and thrive in their surroundings through natural selection and genetic algorithms.

## The Concept
The bots are spawned within a defined environment size. Currently after about 0.75 sedoncs new food is spawned. The bots loose each 10 seoncds one foodpoint. If they drop under 0 foodpoints they die. If there are not enough bots, new ones are spawn the prevent the population from dieng out. If they eat food they gain foodpoints. To see the world they have several sensos and get information such as if there is a wall, food or another bot and the distance to the object. When two bots are close enough and have enough energy they can mate and create a new bot with mixed traits from both parents. Over time, the bots that are better at finding food and surviving will reproduce more, leading to an evolution of the population. Never the less this also means that the food sources are harder to find, so the bots are also competitors to each other.

Even after a short simulation to can find realy smart bots that are able to find food and survive for a long time. Never the less the project has much room to improve and will never be really "finished". Currently it is a simple linear neuronal network that controls the bots based on their inputs. Future plans include adding more complex behaviors, memory neurons, environmental challenges, and possibly even allowing users to interact with the simulation ...

## Controls
- drag the mouse to move the camera
- scroll to zoom in and out
- use the slider to adjust the simulation speed

## How to build
1. Clone the repository
2. Ensure you have Godot Engine 4 installed on your machine
3. Open the project in Godot Engine
4. Run the project to start the simulation

## Note
Older versions used c++ and SFML. The new version uses Godot Engine 4. The reason for this change is that Godot provides a more integrated and user-friendly environment for game development, allowing for faster prototyping and iteration. Additionally, Godot's built-in physics engine and scene system simplify the process of creating and managing game objects, making it easier to focus on gameplay mechanics rather than low-level implementation details. This transition aims to enhance productivity and streamline the development workflow.