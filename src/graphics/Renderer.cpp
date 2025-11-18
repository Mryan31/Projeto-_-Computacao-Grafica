// src/graphics/Renderer.cpp
#include "graphics/Renderer.hpp"
#include "graphics/Shadow.hpp"
#include <glad/glad.h>   // Para comandos OpenGL (glBegin, glEnd, etc.)
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

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
        drawBoidShape(boid);

        // Restaura o estado do mundo para o próximo boid
        glPopMatrix();
    }

    // Desenha o Boid-Objetivo (Líder)
    glm::vec3 goalPos = flock.getGoalPosition();
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho

    glPushMatrix();
        glTranslatef(goalPos.x, goalPos.y, goalPos.z);
        // TODO: Rotacionar o líder
        drawBoidShape(flock.boids.empty() ? Boid() : flock.boids[0]); // <-- MUDANÇA AQUI
    glPopMatrix();
}

// Esta função desenha o poliedro 3D
void Renderer::drawBoidShape(const Boid& boid) {
    float size = 0.3f; // Tamanho do boid

    // --- Cálculo da Animação ---
    // Usa 'sin' para fazer o ângulo oscilar entre -1 e 1
    // Multiplica por 45.0f para fazer a asa bater 45 graus
    float flapAngle = sin(glm::radians(boid.wingAngle)) * 45.0f;

    
    // --- Desenha o Corpo (Uma pirâmide fina) ---
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 0.0f, -1.0f); // Bico
        glVertex3f(0.0f, 0.05f * size, -size); // Bico (ponta de cima)

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.1f * size, 0.0f); // Topo
        
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.1f * size, 0.0f, 0.0f); // Lado direito

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, -0.1f * size, 0.1f * size); // Fundo
        
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.1f * size, 0.0f, 0.0f); // Lado esquerdo

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.1f * size, 0.0f); // Topo (fecha o fan)
    glEnd();

    // --- Asa Esquerda ---
    glPushMatrix();
        // 1. Gira a asa para bater
        glRotatef(flapAngle, 0.0f, 0.0f, 1.0f); // Gira em torno de Z
        // 2. Desenha a asa
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-size, 0.0f, 0.0f);
            glVertex3f(-size, 0.0f, 0.2f * size);
        glEnd();
    glPopMatrix();

    // --- Asa Direita ---
    glPushMatrix();
        // 1. Gira a asa (na direção oposta)
        glRotatef(-flapAngle, 0.0f, 0.0f, 1.0f); // Gira em torno de Z
        // 2. Desenha a asa
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(size, 0.0f, 0.0f);
            glVertex3f(size, 0.0f, 0.2f * size);
        glEnd();
    glPopMatrix();
}

void Renderer::drawBoidGeometry(const Boid& boid) {
    // Chama a função original (ela já desenha a geometria completa)
    drawBoidShape(boid);
}

void Renderer::drawShadows(const Flock& flock, const glm::vec3& lightDir) {
    glm::vec3 normalizedLight = glm::normalize(lightDir);
    glm::mat4 shadowMatrix = Shadow::getShadowMatrix(normalizedLight, 0.0f);
    Shadow::beginShadowRender(0.4f);
    
    // Desenha sombra de cada boid
    for (const Boid& boid : flock.boids) {
        glPushMatrix();
        glTranslatef(boid.position.x, boid.position.y, boid.position.z);
        glTranslatef(0.0f, 0.01f, 0.0f);
        glMultMatrixf(glm::value_ptr(shadowMatrix));
        drawBoidGeometry(boid);
        glPopMatrix();
    }
    
    // Desenha sombra do boid-objetivo (líder)
    glm::vec3 goalPos = flock.getGoalPosition();
    glPushMatrix();
        glTranslatef(goalPos.x, goalPos.y, goalPos.z);
        glTranslatef(0.0f, 0.01f, 0.0f);
        glMultMatrixf(glm::value_ptr(shadowMatrix));
        
        Boid dummyBoid;
        dummyBoid.wingAngle = 0.0f;
        drawBoidGeometry(dummyBoid);
    glPopMatrix();
    
    // Restaura o estado do OpenGL
    Shadow::endShadowRender();
}