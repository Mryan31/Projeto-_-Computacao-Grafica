// src/graphics/World.cpp
#include "graphics/World.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

World::World() {
    // Construtor
}

void World::draw() {
    // TODO: Implementar o desenho do chão (plano) e da torre (cone)
    
    // Por enquanto, desenha um "chão" verde simples
    glColor3f(0.1f, 0.5f, 0.1f); // Cor verde-escuro
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f); // Normal apontando para cima (para luz)
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();
}