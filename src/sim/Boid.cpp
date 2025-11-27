#include "sim/Boid.hpp"
#include <cstdlib> 
#include <ctime>

float RAIO_PERCEPCAO = 8.0f;

float RAIO_SEPARACAO = 1.8f; 

float PESO_ALINHAMENTO = 0.8f;
float PESO_COESAO = 0.2f;      // Reduzido para priorizar seguir o líder
float PESO_SEPARACAO = 1.5f;   // Reduzido para permitir formação mais compacta
float PESO_SEEK_GOAL = 8.0f;   // Muito aumentado para seguir o líder rapidamente 

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
    
    // DESVIO DA TORRE COM PREVISÃO DE TRAJETÓRIA
    glm::vec3 towerCenter(0.0f, 0.0f, 0.0f);
    float towerBaseRadius = 5.0f;
    float towerHeight = 30.0f;
    
    // Calcular posição futura (lookahead)
    float lookAheadTime = 2.0f; // Olhar 2 segundos à frente
    glm::vec3 futurePos = position + velocity * lookAheadTime;
    
    // Distâncias atual e futura
    glm::vec2 horizontalPos(position.x, position.z);
    glm::vec2 horizontalFuture(futurePos.x, futurePos.z);
    glm::vec2 horizontalTower(towerCenter.x, towerCenter.z);
    
    float currentDist = glm::distance(horizontalPos, horizontalTower);
    float futureDist = glm::distance(horizontalFuture, horizontalTower);
    
    // Raio de detecção expandido
    float detectionRadius = 20.0f;
    float criticalRadius = 10.0f;
    
    // Se está se aproximando da torre (posição atual ou futura)
    if ((currentDist < detectionRadius || futureDist < detectionRadius) && 
        (position.y < towerHeight + 5.0f || futurePos.y < towerHeight + 5.0f)) {
        
        // Calcular raio do cone nas alturas relevantes
        float coneRadiusAtCurrentHeight = towerBaseRadius * glm::max(0.0f, 1.0f - position.y / towerHeight);
        float coneRadiusAtFutureHeight = towerBaseRadius * glm::max(0.0f, 1.0f - futurePos.y / towerHeight);
        
        // Zona de perigo expandida
        float dangerRadiusCurrent = coneRadiusAtCurrentHeight + 8.0f;
        float dangerRadiusFuture = coneRadiusAtFutureHeight + 8.0f;
        
        bool inDangerNow = currentDist < dangerRadiusCurrent;
        bool inDangerFuture = futureDist < dangerRadiusFuture;
        bool headingTowards = futureDist < currentDist; // Indo em direção à torre
        
        if (inDangerNow || inDangerFuture || (headingTowards && currentDist < detectionRadius)) {
            // Direção para longe da torre
            glm::vec2 awayDirection;
            if (currentDist > 0.1f) {
                awayDirection = glm::normalize(horizontalPos - horizontalTower);
            } else {
                // Empurra para a direita se está no centro
                awayDirection = glm::vec2(1.0f, 0.0f);
            }
            
            // Calcular força baseada em múltiplos fatores
            float forceMultiplier = 1.0f;
            
            // PERIGO CRÍTICO - dentro da zona
            if (currentDist < criticalRadius) {
                forceMultiplier = 30.0f * (1.0f - currentDist / criticalRadius);
            }
            // Zona de alerta
            else if (currentDist < detectionRadius) {
                forceMultiplier = 15.0f * (1.0f - currentDist / detectionRadius);
            }
            
            // Bônus se estiver indo em direção à torre
            if (headingTowards) {
                forceMultiplier *= 2.0f;
            }
            
            // FORÇA MASSIVA para desvio
            glm::vec3 avoidForce(awayDirection.x * 2.0f, 2.0f, awayDirection.y * 2.0f);
            avoidForce = glm::normalize(avoidForce) * maxSpeed * forceMultiplier;
            
            steer += avoidForce;
        }
    }
    
    if (glm::length(steer) > 0.0f) {
        if (glm::length(steer) > maxSpeed * 3.0f) {
            steer = glm::normalize(steer) * maxSpeed * 3.0f; // Permite força até 3x maxSpeed
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

    maxSpeed = 7.0f;  // Aumentado ainda mais para acompanhar o líder
    maxForce = 0.3f;  // MUITO aumentado para permitir manobras bruscas de desvio

    wingAngle = boidRandomFloat() * 180.0f; 
    wingSpeed = 500.0f + (boidRandomFloat() * 200.0f);
}

// O "Músculo"
void Boid::applyForce(const glm::vec3& force) {
    acceleration += force;
}

// O "Cérebro"
void Boid::calculateForces(const std::vector<Boid>& flock, const glm::vec3& goalPosition, const glm::vec3& goalVelocity) {
    acceleration = glm::vec3(0.0f);

    glm::vec3 sep = separate(flock);
    glm::vec3 ali = align(flock);
    glm::vec3 coh = cohere(flock);
    glm::vec3 bounds = checkBounds();

    // Calcular posição de formação em V: atrás e nas laterais do líder
    glm::vec3 toGoal = goalPosition - position;
    float distanceToGoal = glm::length(toGoal);
    
    glm::vec3 seek;
    if (distanceToGoal > 0.1f) {
        // Direção do movimento do líder
        glm::vec3 leaderDirection;
        if (glm::length(goalVelocity) > 0.1f) {
            leaderDirection = glm::normalize(goalVelocity);
        } else {
            // Se líder está parado, usa direção para o boid como referência
            leaderDirection = -glm::normalize(toGoal);
        }
        
        // Calcular posição única de formação baseada na posição inicial do boid
        float boidIndex = (position.x + position.z) * 0.5f; // Hash simples baseado na posição
        float formationDistance = 2.0f + fmod(abs(boidIndex), 2.0f) * 1.5f; // 2 a 5 unidades atrás (mais próximo)
        float lateralOffset = sin(boidIndex) * 3.0f; // -3 a +3 nas laterais (formação mais compacta)
        float verticalOffset = -0.3f + fmod(abs(boidIndex * 1.3f), 0.6f); // Pequena variação vertical
        
        // Vetores perpendiculares à direção do líder
        glm::vec3 right = glm::cross(leaderDirection, glm::vec3(0.0f, 1.0f, 0.0f));
        if (glm::length(right) < 0.01f) {
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        } else {
            right = glm::normalize(right);
        }
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        // Posição de formação: atrás + lateral + vertical
        glm::vec3 formationPos = goalPosition 
            - leaderDirection * formationDistance  // Atrás do líder
            + right * lateralOffset                // Lateral (formação em V)
            + up * verticalOffset;                 // Pequena variação em altura
        
        // Buscar a posição de formação
        seek = steerTo(formationPos);
        
        // Se estiver longe, aumentar progressivamente a força
        if (distanceToGoal > 10.0f) {
            float multiplier = 1.0f + (distanceToGoal - 10.0f) * 0.3f; // Aumenta progressivamente
            seek = seek * multiplier;
        }
        if (distanceToGoal > 25.0f) {
            seek = steerTo(goalPosition) * 3.0f; // Muito longe: ir direto para o líder com força triplicada
        }
    } else {
        seek = glm::vec3(0.0f);
    }

    applyForce(sep * PESO_SEPARACAO);
    applyForce(ali * PESO_ALINHAMENTO);
    applyForce(coh * PESO_COESAO);
    applyForce(bounds * 10.0f); // MUITO aumentado - desvio de obstáculos tem PRIORIDADE MÁXIMA
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