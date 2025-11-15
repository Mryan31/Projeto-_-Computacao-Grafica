#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "sim/Flock.hpp" // Precisa saber o que é um "Flock"

class Renderer {
public:
    Renderer();

    // Função principal: desenha todos os boids do bando
    void draw(const Flock& flock);

private:
    // Função helper: desenha um único boid na origem (0,0,0)
    void drawBoidShape(const Boid& boid);
};

#endif