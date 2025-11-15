#include "sim/Boid.hpp"
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()

// --- Parâmetros de Simulação ---
// (Você pode "brincar" com estes números para mudar o comportamento do bando)

// Quão longe um boid pode "ver" seus vizinhos
float RAIO_PERCEPCAO = 8.0f;

// O "espaço pessoal" que um boid tenta manter
float RAIO_SEPARACAO = 1.8f; 

// Pesos para cada força (quão importante cada regra é)
float PESO_ALINHAMENTO = 1.0f;
float PESO_COESAO = 0.4f;      
float PESO_SEPARACAO = 2.0f; 
float PESO_SEEK_GOAL = 1.5f; 

// --- Funções Helper (privadas) ---

// (A função randomFloat() foi movida para Flock.cpp)

// Calcula uma força de esterço (steering) em direção a um alvo
// Esta é a fórmula clássica de Reynolds: Força = Desejado - Atual
glm::vec3 Boid::steerTo(const glm::vec3& target) {
    // 1. Calcula o vetor de velocidade "desejada"
    glm::vec3 desired = target - position;
    // Se a distância for 0, não faça nada
    if (glm::length(desired) == 0.0f) {
        return glm::vec3(0.0f);
    }
    
    // 2. Normaliza e dimensiona para a velocidade máxima
    desired = glm::normalize(desired) * maxSpeed;

    // 3. Calcula a força de esterço (Steering = Desired - Current)
    glm::vec3 steer = desired - velocity;

    // 4. Limita a força (para não virar muito rápido)
    if (glm::length(steer) > maxForce) {
        steer = glm::normalize(steer) * maxForce;
    }
    return steer;
}

// Regra 1: SEPARAÇÃO
// Evitar colidir com vizinhos próximos
glm::vec3 Boid::separate(const std::vector<Boid>& flock) {
    glm::vec3 steer(0.0f);
    int count = 0;

    for (const Boid& other : flock) {
        float d = glm::distance(position, other.position);
        // Se o vizinho está dentro do nosso "espaço pessoal"
        if (d > 0 && d < RAIO_SEPARACAO) {
            // Calcula um vetor apontando para longe do vizinho
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
    
    // Se houver uma força de separação, aplica-a
    if (glm::length(steer) > 0.0f) {
        steer = glm::normalize(steer) * maxSpeed; // Velocidade desejada
        steer = steer - velocity;                 // Força de esterço
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce; // Limita
        }
    }
    return steer;
}

// Regra 2: ALINHAMENTO
// Tentar se mover na mesma direção que os vizinhos
glm::vec3 Boid::align(const std::vector<Boid>& flock) {
    glm::vec3 sum(0.0f);
    int count = 0;

    for (const Boid& other : flock) {
        float d = glm::distance(position, other.position);
        // Se é um vizinho (dentro do raio de percepção)
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

// Regra 3: COESÃO
// Tentar se mover em direção ao centro do bando de vizinhos
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

// Helper: Calcula uma força de esterço para longe dos limites
glm::vec3 Boid::checkBounds() {
    glm::vec3 steer(0.0f);
    float boundary = 3.0f; // Distância de "perigo" do chão
    float ceiling = 30.0f; // Teto

    // Força para evitar o chão
    if (position.y < boundary) {
        // Calcula uma força de esterço para CIMA
        glm::vec3 desired = glm::vec3(velocity.x, maxSpeed, velocity.z);
        steer = desired - velocity;
    }
    // Força para evitar o teto
    else if (position.y > ceiling) {
        // Calcula uma força de esterço para BAIXO
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


// --- Funções Públicas ---

// Função helper (privada) para 'Boid()'
// (Não pode ser 'randomFloat' pois movemos para Flock.cpp)
static float boidRandomFloat() {
    return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
}

Boid::Boid() {
    // Semente do gerador aleatório
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    // Posição inicial aleatória
    position = glm::vec3(
        boidRandomFloat() * 20.0f,
        boidRandomFloat() * 10.0f + 10.0f,
        boidRandomFloat() * 20.0f
    );

    // Velocidade inicial aleatória
    velocity = glm::vec3(boidRandomFloat(), boidRandomFloat(), boidRandomFloat());
    velocity = glm::normalize(velocity) * 2.0f;

    // Reseta a aceleração
    acceleration = glm::vec3(0.0f);

    // Define os limites
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
    // Reseta a aceleração para este frame
    acceleration = glm::vec3(0.0f);

    // Calcula as 3 forças das regras
    glm::vec3 sep = separate(flock);
    glm::vec3 ali = align(flock);
    glm::vec3 coh = cohere(flock);
    glm::vec3 bounds = checkBounds();

    // Regra 4: "Seek Goal" (Seguir o Líder)
    glm::vec3 seek = steerTo(goalPosition);

    // Aplica as forças com seus pesos
    applyForce(sep * PESO_SEPARACAO);
    applyForce(ali * PESO_ALINHAMENTO);
    applyForce(coh * PESO_COESAO);
    applyForce(bounds * 1.5f); // Multiplicador ajustado
    applyForce(seek * PESO_SEEK_GOAL);
}

// A "Física"
void Boid::update(float deltaTime) {
    // 1. Atualiza a velocidade com a aceleração
    velocity += acceleration * deltaTime;

    // 2. Limita a velocidade
    if (glm::length(velocity) > maxSpeed) {
        velocity = glm::normalize(velocity) * maxSpeed;
    }

    // 3. Atualiza a posição com a velocidade
    position += velocity * deltaTime;

    // (Opcional) Fazer o bando "dar a volta" se sair da tela
    if (position.x > 100.0f) position.x = -100.0f;
    if (position.x < -100.0f) position.x = 100.0f;
    if (position.z > 100.0f) position.z = -100.0f;
    if (position.z < -100.0f) position.z = 100.0f;

    wingAngle += wingSpeed * deltaTime;
}