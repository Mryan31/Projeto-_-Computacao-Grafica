// src/graphics/Renderer.cpp
#include "graphics/Renderer.hpp"
#include <glad/glad.h>   // Para comandos OpenGL (glBegin, glEnd, etc.)
#include <GLFW/glfw3.h>  // (Incluído por 'glad.h' geralmente, mas por via das dúvidas)

Renderer::Renderer() {
    // Construtor (pode ser usado para carregar texturas, etc.)
}

void Renderer::draw(const Flock& flock) {
    // Define a cor dos boids (amarelo)
    glColor3f(1.0f, 1.0f, 0.0f);

    // Loop por todos os boids no bando
    for (const Boid& boid : flock.boids) {
        
        // Salva o estado atual do mundo (matriz)
        glPushMatrix();

        // 1. Move o "cursor" de desenho para a posição do boid
        glTranslatef(boid.position.x, boid.position.y, boid.position.z);

        // 2. TODO: Rotacionar o boid para que ele aponte na direção 
        //    da sua 'boid.velocity'. (Isso é mais avançado)

        // 3. Desenha a forma 3D do boid
        drawBoidShape();

        // Restaura o estado do mundo para o próximo boid
        glPopMatrix();
    }
}

// Esta função desenha o poliedro 3D [cite: 21]
void Renderer::drawBoidShape() {
    // Desenha uma forma de "pássaro" muito simples, parecida com
    // uma pirâmide/cone, como sugerido no PDF [cite: 22]
    // A forma aponta para a direção -Z (frente)
    
    float size = 0.3f; // Tamanho do boid

    glBegin(GL_TRIANGLES);
        // "Asa" Esquerda
        glVertex3f( 0.0f,  0.0f, 0.0f);
        glVertex3f(-size,  0.0f, 0.1f * size);
        glVertex3f( 0.0f,  0.1f * size, 0.0f);

        // "Asa" Direita
        glVertex3f( 0.0f,  0.0f, 0.0f);
        glVertex3f( 0.0f,  0.1f * size, 0.0f);
        glVertex3f( size,  0.0f, 0.1f * size);
        
        // "Cauda"
        glVertex3f( 0.0f,  0.0f, 0.0f);
        glVertex3f( 0.0f,  0.1f * size, 0.0f);
        glVertex3f( 0.0f,  0.0f, 0.3f * size); // Parte de trás

        // "Bico" / "Frente" (Pirâmide)
        glVertex3f( 0.0f,  0.0f, -size); // Ponto da frente
        glVertex3f(-size,  0.0f, 0.1f * size);
        glVertex3f( 0.0f,  0.1f * size, 0.0f);

        glVertex3f( 0.0f,  0.0f, -size); // Ponto da frente
        glVertex3f( 0.0f,  0.1f * size, 0.0f);
        glVertex3f( size,  0.0f, 0.1f * size);

    glEnd();
}