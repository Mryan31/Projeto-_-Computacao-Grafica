// src/graphics/Shadow.cpp
#include "graphics/Shadow.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 Shadow::getShadowMatrix(const glm::vec3& lightDir, float planeY) {
    glm::vec3 l = glm::normalize(lightDir);
    glm::vec3 n(0.0f, 1.0f, 0.0f);
    
    float dot = glm::dot(n, l);
    
    glm::mat4 shadowMat(1.0f);
    
    shadowMat[0][0] = dot - l.x * n.x;
    shadowMat[1][0] = -l.x * n.y;
    shadowMat[2][0] = -l.x * n.z;
    shadowMat[3][0] = -l.x * planeY;
    
    shadowMat[0][1] = -l.y * n.x;
    shadowMat[1][1] = dot - l.y * n.y;
    shadowMat[2][1] = -l.y * n.z;
    shadowMat[3][1] = -l.y * planeY;
    
    shadowMat[0][2] = -l.z * n.x;
    shadowMat[1][2] = -l.z * n.y;
    shadowMat[2][2] = dot - l.z * n.z;
    shadowMat[3][2] = -l.z * planeY;
    
    shadowMat[0][3] = -n.x;
    shadowMat[1][3] = -n.y;
    shadowMat[2][3] = -n.z;
    shadowMat[3][3] = dot - planeY;
    
    return shadowMat;
}

void Shadow::beginShadowRender(float alpha) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    glDepthMask(GL_FALSE);
}

void Shadow::endShadowRender() {
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}
