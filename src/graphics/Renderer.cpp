// src/graphics/Renderer.cpp
#include "graphics/Renderer.hpp"
#include "graphics/Shadow.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {
}

void Renderer::draw(const Flock& flock) {
    glColor3f(1.0f, 1.0f, 0.0f);

    for (const Boid& boid : flock.boids) {
        
        glPushMatrix();

        glTranslatef(boid.position.x, boid.position.y, boid.position.z);

        drawBoidShape(boid);

        glPopMatrix();
    }

    glm::vec3 goalPos = flock.getGoalPosition();
    glColor3f(1.0f, 0.0f, 0.0f); // Vermelho

    glPushMatrix();
        glTranslatef(goalPos.x, goalPos.y, goalPos.z);
        drawBoidShape(flock.boids.empty() ? Boid() : flock.boids[0]);
    glPopMatrix();
}

void Renderer::drawBoidShape(const Boid& boid) {
    float size = 0.3f; // Tamanho do boid

    float flapAngle = sin(glm::radians(boid.wingAngle)) * 45.0f;

    
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

    glPushMatrix();
        glRotatef(flapAngle, 0.0f, 0.0f, 1.0f); // Gira em torno de Z
        
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(-size, 0.0f, 0.0f);
            glVertex3f(-size, 0.0f, 0.2f * size);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glRotatef(-flapAngle, 0.0f, 0.0f, 1.0f); // Gira em torno de Z
        
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(size, 0.0f, 0.0f);
            glVertex3f(size, 0.0f, 0.2f * size);
        glEnd();
    glPopMatrix();
}

void Renderer::drawBoidGeometry(const Boid& boid) {
    drawBoidShape(boid);
}

void Renderer::drawShadows(const Flock& flock, const glm::vec3& lightDir) {
    glm::vec3 normalizedLight = glm::normalize(lightDir);
    glm::mat4 shadowMatrix = Shadow::getShadowMatrix(normalizedLight, 0.0f);
    Shadow::beginShadowRender(0.4f);
    
    for (const Boid& boid : flock.boids) {
        glPushMatrix();
        glTranslatef(boid.position.x, boid.position.y, boid.position.z);
        glTranslatef(0.0f, 0.01f, 0.0f);
        glMultMatrixf(glm::value_ptr(shadowMatrix));
        drawBoidGeometry(boid);
        glPopMatrix();
    }
    
    glm::vec3 goalPos = flock.getGoalPosition();
    glPushMatrix();
        glTranslatef(goalPos.x, goalPos.y, goalPos.z);
        glTranslatef(0.0f, 0.01f, 0.0f);
        glMultMatrixf(glm::value_ptr(shadowMatrix));
        
        Boid dummyBoid;
        dummyBoid.wingAngle = 0.0f;
        drawBoidGeometry(dummyBoid);
    glPopMatrix();
    
    Shadow::endShadowRender();
}