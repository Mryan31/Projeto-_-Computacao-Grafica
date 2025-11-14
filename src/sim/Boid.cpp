#include "sim/Boid.hpp"

#include <cstdlib>
#include <ctime>

//Gera um float aleatório entre -1.0 e 1.0
float randomFloat() {
    return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
}

Boid::Boid() {
    // Inicializa o gerador de números aleatórios (só precisa ser feito uma vez,
    // mas aqui é simples). Uma solução melhor seria um gerador estático.
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    // Posição aleatória em um cubo de -10 a +10
    position = glm::vec3(
        randomFloat() * 10.0f,
        randomFloat() * 10.0f + 10.0f, // Começa mais alto (no ar)
        randomFloat() * 10.0f
    );

    // Velocidade aleatória, mas geralmente indo para frente (eixo -Z, por exemplo)
    velocity = glm::vec3(
        randomFloat() * 0.5f, // Pequena variação lateral
        randomFloat() * 0.2f, // Pequena variação vertical
        -1.0f                 // Movimento principal para frente
    );
    
    // Normaliza a velocidade e define uma magnitude (velocidade)
    velocity = glm::normalize(velocity) * 2.0f; // 2.0f = velocidade de 2 unidades/segundo
}

void Boid::update(float deltaTime) {
    // Lógica de movimento MUITO simples por enquanto:
    // Posição = Posição anterior + Velocidade * Tempo
    position += velocity * deltaTime;

    // TODO: A lógica real (Separação, Coesão, Alinhamento)
    // será implementada aqui depois.
}