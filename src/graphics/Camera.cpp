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
    mode = (mode + 1) % 3; // Alterna entre 0, 1, 2
    if (mode == 0) std::cout << "Modo Camera: Torre" << std::endl;
    if (mode == 1) std::cout << "Modo Camera: Atras do Bando" << std::endl;
    if (mode == 2) std::cout << "Modo Camera: Lateral" << std::endl;
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
    else { 
        eyePosition = flock.center + glm::vec3(25.0f, 3.0f, 0.0f);
    }


    gluLookAt(
        eyePosition.x, eyePosition.y, eyePosition.z, // Posição do "olho"
        lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, // Ponto para onde a câmera olha
        upVector.x,    upVector.y,    upVector.z     // Vetor "para cima"
    );
}