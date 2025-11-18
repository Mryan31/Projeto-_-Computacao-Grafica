#include "sim/Flock.hpp"
#include <iostream>
#include <cstdlib>


float randomFloat() {
    return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
}

Flock::Flock(int startSize) : 
    center(0.0f, 0.0f, 0.0f),
    goalPosition(0.0f, 10.0f, 0.0f), // Começa no ar, na torre
    goalVelocity(0.0f)
{
    for (int i = 0; i < startSize; ++i) {
        boids.push_back(Boid());
    }
}

void Flock::setGoalVelocity(const glm::vec3& vel) {
    goalVelocity = vel;
}

glm::vec3 Flock::getGoalPosition() const {
    return goalPosition;
}

void Flock::addBoid() {
    Boid newBoid;
    
    newBoid.position = center + glm::vec3(
        randomFloat(),
        randomFloat(),
        randomFloat()
    );
    
    boids.push_back(newBoid);
    std::cout << "Boid adicionado. Total: " << boids.size() << std::endl;
}

void Flock::removeBoid() {
    if (!boids.empty()) {
        boids.pop_back();
        std::cout << "Boid removido. Total: " << boids.size() << std::endl;
    }
}


void Flock::update(float deltaTime) {
    
    goalPosition += goalVelocity * deltaTime;

    if (goalPosition.y < 2.0f) goalPosition.y = 2.0f;
    if (goalPosition.y > 40.0f) goalPosition.y = 40.0f;
    if (goalPosition.x > 100.0f) goalPosition.x = -100.0f;
    if (goalPosition.x < -100.0f) goalPosition.x = 100.0f;
    if (goalPosition.z > 100.0f) goalPosition.z = -100.0f;
    if (goalPosition.z < -100.0f) goalPosition.z = 100.0f;


    for (Boid& boid : boids) {
        boid.calculateForces(boids, goalPosition);
    }

    glm::vec3 sumPositions(0.0f);
    if (boids.empty()) {
        center = glm::vec3(0.0f);
        return;
    }

    for (Boid& boid : boids) {
        boid.update(deltaTime);
        sumPositions += boid.position;
    }

    center = sumPositions / (float)boids.size();
}