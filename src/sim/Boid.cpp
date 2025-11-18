#include "sim/Boid.hpp"
#include <cstdlib> 
#include <ctime>

float RAIO_PERCEPCAO = 8.0f;

float RAIO_SEPARACAO = 1.8f; 

float PESO_ALINHAMENTO = 1.0f;
float PESO_COESAO = 0.4f;      
float PESO_SEPARACAO = 2.0f; 
float PESO_SEEK_GOAL = 1.5f; 

glm::vec3 Boid::steerTo(const glm::vec3& target) {
    glm::vec3 desired = target - position;
    if (glm::length(desired) == 0.0f) {
        return glm::vec3(0.0f);
    }
    
    desired = glm::normalize(desired) * maxSpeed;

    glm::vec3 steer = desired - velocity;

    if (glm::length(steer) > maxForce) {
        steer = glm::normalize(steer) * maxForce;
    }
    return steer;
}

glm::vec3 Boid::separate(const std::vector<Boid>& flock) {
    glm::vec3 steer(0.0f);
    int count = 0;

    for (const Boid& other : flock) {
        float d = glm::distance(position, other.position);
        if (d > 0 && d < RAIO_SEPARACAO) {
            glm::vec3 diff = position - other.position;
            diff = glm::normalize(diff);
            diff /= d; // Pondera pela distância (quanto mais perto, mais forte)
            steer += diff;
            count++;
        }
    }

    if (count > 0) {
        steer /= (float)count;
    }
    
    if (glm::length(steer) > 0.0f) {
        steer = glm::normalize(steer) * maxSpeed; // Velocidade desejada
        steer = steer - velocity;                 // Força de esterço
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce; // Limita
        }
    }
    return steer;
}

glm::vec3 Boid::align(const std::vector<Boid>& flock) {
    glm::vec3 sum(0.0f);
    int count = 0;

    for (const Boid& other : flock) {
        float d = glm::distance(position, other.position);
        if (d > 0 && d < RAIO_PERCEPCAO) {
            sum += other.velocity; // Soma as velocidades
            count++;
        }
    }

    if (count > 0) {
        sum /= (float)count; // Pega a velocidade média
        sum = glm::normalize(sum) * maxSpeed; // Velocidade desejada
        glm::vec3 steer = sum - velocity;     // Força de esterço
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce; // Limita
        }
        return steer;
    }
    return glm::vec3(0.0f);
}

glm::vec3 Boid::cohere(const std::vector<Boid>& flock) {
    glm::vec3 sum(0.0f);
    int count = 0;

    for (const Boid& other : flock) {
        float d = glm::distance(position, other.position);
        if (d > 0 && d < RAIO_PERCEPCAO) {
            sum += other.position; // Soma as posições
            count++;
        }
    }

    if (count > 0) {
        sum /= (float)count; // Pega a posição média (o "centro")
        return steerTo(sum); // Usa nossa função helper para ir até lá
    }
    return glm::vec3(0.0f);
}

glm::vec3 Boid::checkBounds() {
    glm::vec3 steer(0.0f);
    float boundary = 3.0f; // Distância de "perigo" do chão
    float ceiling = 30.0f; // Teto

    if (position.y < boundary) {
        glm::vec3 desired = glm::vec3(velocity.x, maxSpeed, velocity.z);
        steer = desired - velocity;
    }
    else if (position.y > ceiling) {
        glm::vec3 desired = glm::vec3(velocity.x, -maxSpeed, velocity.z);
        steer = desired - velocity;
    }
    
    if (glm::length(steer) > 0.0f) {
        steer = glm::normalize(steer) * maxSpeed;
        steer -= velocity; // Força de esterço
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce;
        }
    }
    return steer;
}

static float boidRandomFloat() {
    return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
}

Boid::Boid() {
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    position = glm::vec3(
        boidRandomFloat() * 20.0f,
        boidRandomFloat() * 10.0f + 10.0f,
        boidRandomFloat() * 20.0f
    );

    velocity = glm::vec3(boidRandomFloat(), boidRandomFloat(), boidRandomFloat());
    velocity = glm::normalize(velocity) * 2.0f;

    acceleration = glm::vec3(0.0f);

    maxSpeed = 3.5f;
    maxForce = 0.05f;

    wingAngle = boidRandomFloat() * 180.0f; 
    wingSpeed = 500.0f + (boidRandomFloat() * 200.0f);
}

// O "Músculo"
void Boid::applyForce(const glm::vec3& force) {
    acceleration += force;
}

// O "Cérebro"
void Boid::calculateForces(const std::vector<Boid>& flock, const glm::vec3& goalPosition) {
    acceleration = glm::vec3(0.0f);

    glm::vec3 sep = separate(flock);
    glm::vec3 ali = align(flock);
    glm::vec3 coh = cohere(flock);
    glm::vec3 bounds = checkBounds();

    glm::vec3 seek = steerTo(goalPosition);

    applyForce(sep * PESO_SEPARACAO);
    applyForce(ali * PESO_ALINHAMENTO);
    applyForce(coh * PESO_COESAO);
    applyForce(bounds * 1.5f); // Multiplicador ajustado
    applyForce(seek * PESO_SEEK_GOAL);
}

// A "Física"
void Boid::update(float deltaTime) {
    velocity += acceleration * deltaTime;

    if (glm::length(velocity) > maxSpeed) {
        velocity = glm::normalize(velocity) * maxSpeed;
    }

    position += velocity * deltaTime;

    if (position.x > 100.0f) position.x = -100.0f;
    if (position.x < -100.0f) position.x = 100.0f;
    if (position.z > 100.0f) position.z = -100.0f;
    if (position.z < -100.0f) position.z = 100.0f;

    wingAngle += wingSpeed * deltaTime;
}