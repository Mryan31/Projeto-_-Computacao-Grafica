//Vai conter um std::vector<Boid> (ou similar). A lógica de Separação , Coesão e Alinhamento  será implementada aqui, iterando sobre todos os boids.
#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "Boid.hpp"

#include <vector>

class Flock
{
public:
    std::vector<Boid> boids;

    Flock(int startSize = 50);

    void update(float deltaTime);
};



#endif