// src/sim/Flock.cpp
#include "sim/Flock.hpp"

Flock::Flock(int startSize) : center(0.0f, 0.0f, 0.0f) {
    // Cria o número inicial de boids
    for (int i = 0; i < startSize; ++i) {
        boids.push_back(Boid());
    }
}

void Flock::update(float deltaTime) {
    // --- PASSO 1: CALCULAR FORÇAS ---
    // Cada boid calcula suas forças olhando para o bando todo
    // (É importante fazer isso primeiro, antes de qualquer um se mover)
    for (Boid& boid : boids) {
        boid.calculateForces(boids);
    }

    // --- PASSO 2: ATUALIZAR FÍSICA ---
    // Agora que as forças foram calculadas, move todo mundo
    
    // Reseta a soma para recalcular o centro
    glm::vec3 sumPositions(0.0f);
    if (boids.empty()) {
        center = glm::vec3(0.0f);
        return;
    }

    for (Boid& boid : boids) {
        boid.update(deltaTime); // Boid se move
        sumPositions += boid.position; // Adiciona à nova soma
    }

    // Atualiza o centro do bando para a câmera
    center = sumPositions / (float)boids.size();
}