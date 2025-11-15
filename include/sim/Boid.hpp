// Representará um único boid. Vai guardar sua posição, velocidade e o estado da animação das asas.
#ifndef BOID_HPP
#define BOID_HPP

#include <glm/glm.hpp>

#include <vector>

class Boid
{
public:

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float maxSpeed;
    float maxForce;

    float wingAngle; // O ângulo atual da asa
    float wingSpeed; // A velocidade que a asa bate

    Boid();

    void update(float deltaTime);

    void calculateForces(const std::vector<Boid>& flock, const glm::vec3& goalPosition);

    void applyForce(const glm::vec3& force);

private:
    // --- Funções de Regra (Helpers) ---

    // Helper para calcular a força de Separação
    glm::vec3 separate(const std::vector<Boid>& flock);

    // Helper para calcular a força de Alinhamento
    glm::vec3 align(const std::vector<Boid>& flock);

    // Helper para calcular a força de Coesão
    glm::vec3 cohere(const std::vector<Boid>& flock);

    // Função utilitária para calcular a "força de esterço" (steering)
    glm::vec3 steerTo(const glm::vec3& target);

    // Helper para calcular a força de desvio dos limites (chão/teto)
    glm::vec3 checkBounds();
};

#endif