// Representará um único boid. Vai guardar sua posição, velocidade e o estado da animação das asas.
#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>

class Boid
{
public:

    glm::vec3 position;
    glm::vec3 velocity;

    Boid();

    void update(float deltaTime);
};

#endif