#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "sim/Flock.hpp" // Precisa saber o que é um "Flock"
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();

    // Função principal: desenha todos os boids do bando
    void draw(const Flock& flock);

    // Desenha as sombras dos boids no chão
    void drawShadows(const Flock& flock, const glm::vec3& lightDir);

private:
    // Função helper: desenha um único boid na origem (0,0,0)
    void drawBoidShape(const Boid& boid);
    
    // Função auxiliar: desenha apenas a geometria do boid (sem transformações)
    void drawBoidGeometry(const Boid& boid);
};

#endif