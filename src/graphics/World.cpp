#include "graphics/World.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/glu.h>

World::World() {
    quadric = gluNewQuadric(); 
    
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

World::~World() {
    gluDeleteQuadric(quadric);
}

void World::draw() {
    
    int gridSize = 100;
    float spacing = 2.0f;

    glColor3f(0.3f, 0.3f, 0.3f); // Cor cinza escura
    glBegin(GL_LINES);
    
    for (int i = -gridSize; i <= gridSize; i++) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f((float)i * spacing, 0.0f, (float)-gridSize * spacing);
        glVertex3f((float)i * spacing, 0.0f, (float)gridSize * spacing);
    }
    for (int i = -gridSize; i <= gridSize; i++) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f((float)-gridSize * spacing, 0.0f, (float)i * spacing);
        glVertex3f((float)gridSize * spacing, 0.0f, (float)i * spacing);
    }
    glEnd();


    float towerHeight = 30.0f;
    float towerBaseRadius = 5.0f;
    int towerSlices = 32;
    int towerStacks = 5;

    glColor3f(0.5f, 0.5f, 0.5f); // Cor cinza claro
    
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona para o cone ficar "em pé"

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