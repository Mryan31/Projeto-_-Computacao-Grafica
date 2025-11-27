#include "graphics/Camera.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

Camera::Camera() {
    mode = 0; 
    std::cout << "Modo Camera: Torre" << std::endl;
}

void Camera::nextMode() {
    mode = (mode + 1) % 4; // Alterna entre 0, 1, 2, 3
    if (mode == 0) std::cout << "Modo Camera: Torre" << std::endl;
    if (mode == 1) std::cout << "Modo Camera: Atras do Bando" << std::endl;
    if (mode == 2) std::cout << "Modo Camera: Lateral" << std::endl;
    if (mode == 3) std::cout << "Modo Camera: Terceira Pessoa (Lider)" << std::endl;
}

void Camera::look(const Flock& flock) {
    glLoadIdentity();

    glm::vec3 lookAtPoint = flock.center;

    glm::vec3 upVector(0.0f, 1.0f, 0.0f);

    glm::vec3 eyePosition;

    if (mode == 0) {
        eyePosition = glm::vec3(0.0f, 30.0f, 0.1f); // 0.1f para não ficar 100% reto
    }
    else if (mode == 1) {
        eyePosition = flock.center + glm::vec3(0.0f, 5.0f, 20.0f);
    }
    else if (mode == 2) { 
        eyePosition = flock.center + glm::vec3(25.0f, 3.0f, 0.0f);
    }
    else if (mode == 3) {
        // Câmera em terceira pessoa acompanhando o boid líder (vermelho - goalPosition)
        glm::vec3 leaderPos = flock.getGoalPosition();
        
        // Posição fixa atrás e acima do líder (sem rotação)
        float distanceBehind = 8.0f;
        float heightAbove = 4.0f;
        float sideOffset = 2.0f;
        
        // Câmera em posição fixa relativa ao líder
        eyePosition = leaderPos + glm::vec3(sideOffset, heightAbove, distanceBehind);
        
        // Olha diretamente para o líder
        lookAtPoint = leaderPos;
    }


    gluLookAt(
        eyePosition.x, eyePosition.y, eyePosition.z, // Posição do "olho"
        lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, // Ponto para onde a câmera olha
        upVector.x,    upVector.y,    upVector.z     // Vetor "para cima"
    );
}