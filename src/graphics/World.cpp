#include "graphics/World.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

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
    
    // Desenhar a ilha com areia e água
    int gridSize = 100;
    float cellSize = 2.0f;
    float islandRadius = 40.0f; // Raio da ilha (areia)
    float waterRadius = 100.0f; // Raio total (onde começa o fog)
    
    glDisable(GL_LIGHTING); // Desabilitar iluminação para cores puras
    
    // Desenhar células do chão com cores gradientes
    glBegin(GL_QUADS);
    for (int x = -gridSize; x < gridSize; x++) {
        for (int z = -gridSize; z < gridSize; z++) {
            float x1 = (float)x * cellSize;
            float z1 = (float)z * cellSize;
            float x2 = (float)(x + 1) * cellSize;
            float z2 = (float)(z + 1) * cellSize;
            
            // Calcular distância do centro
            float centerX = (x1 + x2) * 0.5f;
            float centerZ = (z1 + z2) * 0.5f;
            float distFromCenter = sqrt(centerX * centerX + centerZ * centerZ);
            
            // Definir cor baseada na distância
            if (distFromCenter < islandRadius) {
                // Areia (bege/amarelo claro) no centro
                float sandFactor = 1.0f - (distFromCenter / islandRadius) * 0.3f;
                glColor3f(0.93f * sandFactor, 0.87f * sandFactor, 0.69f * sandFactor);
            } else {
                // Transição para água
                float waterDist = distFromCenter - islandRadius;
                float waterFactor = glm::min(waterDist / 30.0f, 1.0f);
                
                // Interpolar entre areia e água
                float r = 0.93f * (1.0f - waterFactor) + 0.0f * waterFactor;
                float g = 0.87f * (1.0f - waterFactor) + 0.5f * waterFactor;
                float b = 0.69f * (1.0f - waterFactor) + 0.8f * waterFactor;
                
                // Água azul nas extremidades
                if (waterFactor > 0.5f) {
                    r = 0.0f + (1.0f - waterFactor) * 0.3f;
                    g = 0.4f + (1.0f - waterFactor) * 0.2f;
                    b = 0.7f + (1.0f - waterFactor) * 0.2f;
                }
                
                glColor3f(r, g, b);
            }
            
            // Desenhar quad
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x2, 0.0f, z1);
            glVertex3f(x2, 0.0f, z2);
            glVertex3f(x1, 0.0f, z2);
        }
    }
    glEnd();
    
    glEnable(GL_LIGHTING); // Reabilitar iluminação


    // Torre no centro (agora marrom/pedra)
    float towerHeight = 30.0f;
    float towerBaseRadius = 5.0f;
    int towerSlices = 32;
    int towerStacks = 5;

    glColor3f(0.55f, 0.47f, 0.42f); // Cor marrom pedra
    
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

void World::drawSun() {
    // Posição do sol (mesma da luz)
    float sunX = 80.0f;
    float sunY = 100.0f;
    float sunZ = 60.0f;
    float sunRadius = 15.0f; // Aumentado para ficar mais visível
    
    // Salvar estado do fog e lighting
    GLboolean fogWasEnabled = glIsEnabled(GL_FOG);
    GLboolean lightingWasEnabled = glIsEnabled(GL_LIGHTING);
    
    glDisable(GL_LIGHTING); // Desabilitar iluminação para o sol brilhar
    glDisable(GL_FOG);      // Desabilitar fog para o sol sempre ficar visível
    
    // Desenhar esfera do sol
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo brilhante puro
    glPushMatrix();
    glTranslatef(sunX, sunY, sunZ);
    gluSphere(quadric, sunRadius, 32, 32);
    glPopMatrix();
    
    // Desenhar halo/brilho ao redor do sol
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glColor4f(1.0f, 0.95f, 0.3f, 0.4f); // Amarelo semi-transparente
    glPushMatrix();
    glTranslatef(sunX, sunY, sunZ);
    gluSphere(quadric, sunRadius * 1.8f, 32, 32);
    glPopMatrix();
    
    glDisable(GL_BLEND);
    
    // Restaurar estados anteriores
    if (fogWasEnabled) glEnable(GL_FOG);
    if (lightingWasEnabled) glEnable(GL_LIGHTING);
}