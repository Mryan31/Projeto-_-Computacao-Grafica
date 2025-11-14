#include "sim/Flock.hpp"

Flock::Flock(int startSize) : center(0.0f, 0.0f, 0.0f) { // <-- Inicializa o centro
    for (int i = 0; i < startSize; ++i) {
        boids.push_back(Boid());
    }
}

void Flock::update(float deltaTime) {
    if (boids.empty()) {
        center = glm::vec3(0.0f); // Se não houver boids, o centro é a origem
        return;
    }

    glm::vec3 sumPositions(0.0f);
    for (Boid& boid : boids) {
        boid.update(deltaTime);
        sumPositions += boid.position; // Soma todas as posições
    }

    // Calcula a média
    center = sumPositions / (float)boids.size(); // <-- ADICIONE ESTA LINHA
}