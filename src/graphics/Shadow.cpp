// src/graphics/Shadow.cpp
#include "graphics/Shadow.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

glm::mat4 Shadow::getShadowMatrix(const glm::vec3& lightDir, float planeY) {
    // lightDir é na verdade a posição da luz (80, 100, 60)
    glm::vec3 lightPos = lightDir;
    
    // Normal do plano (chão aponta para cima)
    glm::vec3 planeNormal(0.0f, 1.0f, 0.0f);
    
    // Distância do plano à origem (plano y = planeY)
    float planeD = -planeY;
    
    // Produto escalar: dot = n · L + d
    float dot = planeNormal.x * lightPos.x + 
                planeNormal.y * lightPos.y + 
                planeNormal.z * lightPos.z + planeD;
    
    // Construir matriz de projeção de sombra planar
    // M[i][j] = dot * delta_ij - L[i] * n[j]
    // onde delta_ij = 1 se i==j, 0 caso contrário
    
    glm::mat4 shadowMat;
    
    // Linha 0
    shadowMat[0][0] = dot - lightPos.x * planeNormal.x;
    shadowMat[1][0] = 0.0f - lightPos.x * planeNormal.y;
    shadowMat[2][0] = 0.0f - lightPos.x * planeNormal.z;
    shadowMat[3][0] = 0.0f - lightPos.x * planeD;
    
    // Linha 1
    shadowMat[0][1] = 0.0f - lightPos.y * planeNormal.x;
    shadowMat[1][1] = dot - lightPos.y * planeNormal.y;
    shadowMat[2][1] = 0.0f - lightPos.y * planeNormal.z;
    shadowMat[3][1] = 0.0f - lightPos.y * planeD;
    
    // Linha 2
    shadowMat[0][2] = 0.0f - lightPos.z * planeNormal.x;
    shadowMat[1][2] = 0.0f - lightPos.z * planeNormal.y;
    shadowMat[2][2] = dot - lightPos.z * planeNormal.z;
    shadowMat[3][2] = 0.0f - lightPos.z * planeD;
    
    // Linha 3
    shadowMat[0][3] = 0.0f - planeD * planeNormal.x;
    shadowMat[1][3] = 0.0f - planeD * planeNormal.y;
    shadowMat[2][3] = 0.0f - planeD * planeNormal.z;
    shadowMat[3][3] = dot - planeD * planeD;
    
    return shadowMat;
}

void Shadow::beginShadowRender(float alpha) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glDepthMask(GL_FALSE);
}

void Shadow::endShadowRender() {
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
