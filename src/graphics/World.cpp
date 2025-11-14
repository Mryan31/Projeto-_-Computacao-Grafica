// src/graphics/World.cpp
#include "graphics/World.hpp" // Nosso .hpp (agora limpo)

// Includes do OpenGL, na ordem correta
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// *** A CORREÇÃO ESTÁ AQUI ***
// 1. Inclui o windows.h primeiro para definir CALLBACK, etc.
#ifdef _WIN32
  #include <windows.h>
#endif
// 2. Agora inclui o glu.h, que depende do windows.h
#include <GL/glu.h>
// *** FIM DA CORREÇÃO ***

World::World() {
    // Cria um novo objeto quádrico
    quadric = gluNewQuadric(); 
    
    // Habilita a geração de "Normais".
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

World::~World() {
    // Limpa o objeto quádrico da memória
    gluDeleteQuadric(quadric);
}

void World::draw() {
    
    // --- 1. Desenhando o Chão (Grid) ---
    int gridSize = 100;
    float spacing = 2.0f;

    glColor3f(0.3f, 0.3f, 0.3f); // Cor cinza escura
    glBegin(GL_LINES);
    
    // Linhas ao longo de Z
    for (int i = -gridSize; i <= gridSize; i++) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f((float)i * spacing, 0.0f, (float)-gridSize * spacing);
        glVertex3f((float)i * spacing, 0.0f, (float)gridSize * spacing);
    }
    // Linhas ao longo de X
    for (int i = -gridSize; i <= gridSize; i++) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f((float)-gridSize * spacing, 0.0f, (float)i * spacing);
        glVertex3f((float)gridSize * spacing, 0.0f, (float)i * spacing);
    }
    glEnd();


    // --- 2. Desenhando a Torre (Cone) ---
    float towerHeight = 30.0f;
    float towerBaseRadius = 5.0f;
    int towerSlices = 32;
    int towerStacks = 5;

    glColor3f(0.5f, 0.5f, 0.5f); // Cor cinza claro
    
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para o cone ficar "em pé"

    // Desenha o cone
    gluCylinder(
        quadric,
        towerBaseRadius, // Raio da base
        0.0f,            // Raio do topo (0 para cone)
        towerHeight,     // Altura
        towerSlices,
        towerStacks
    );
    glPopMatrix();
}