//Vai conter um std::vector<Boid> (ou similar). A lógica de Separação , Coesão e Alinhamento  será implementada aqui, iterando sobre todos os boids.
#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Boid.hpp"

#include <vector>
#include <glm/glm.hpp>

class Flock
{
public:
    std::vector<Boid> boids;
    glm::vec3 center;

    Flock(int startSize = 50);

    void update(float deltaTime);
    void setGoalVelocity (const glm::vec3& vel);
    glm::vec3 getGoalPosition() const;

    void addBoid();
    void removeBoid();

private:
    glm::vec3 goalPosition;
    glm::vec3 goalVelocity;
};



#endif