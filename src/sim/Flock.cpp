#include "sim/Flock.hpp"

Flock::Flock(int startSize) {
    // Cria o número inicial de boids
    for (int i = 0; i < startSize; ++i) {
        boids.push_back(Boid());
    }
}

void Flock::update(float deltaTime) {
    // (Mais tarde, a lógica complexa de interação
    //  entre os boids será feita aqui)

    // Por enquanto, apenas atualiza cada boid individualmente
    for (Boid& boid : boids) {
        boid.update(deltaTime);
    }
}