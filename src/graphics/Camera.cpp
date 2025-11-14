#include "graphics/Camera.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Para gluLookAt
#include <glm/gtc/matrix_transform.hpp> // Para glm::lookAt
#include <iostream>

// Inclui o cabeçalho do GLU (necessário para gluLookAt)
// No Windows, isso geralmente é incluído pelo <windows.h> -> <GL/gl.h>
// Se der erro, pode ser necessário incluir <GL/glu.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h> // Descomente se 'gluLookAt' não for encontrado

Camera::Camera() {
    mode = 0; // Começa no modo 0 (Torre)
    std::cout << "Modo Camera: Torre" << std::endl;
}

void Camera::nextMode() {
    mode = (mode + 1) % 3; // Alterna entre 0, 1, 2
    if (mode == 0) std::cout << "Modo Camera: Torre" << std::endl;
    if (mode == 1) std::cout << "Modo Camera: Atras do Bando" << std::endl;
    if (mode == 2) std::cout << "Modo Camera: Lateral" << std::endl;
}

void Camera::look(const Flock& flock) {
    // Reseta a matriz de visão do modelo
    glLoadIdentity();

    // O ponto para onde a câmera SEMPRE olha é o centro do bando 
    glm::vec3 lookAtPoint = flock.center;

    // A normal "up" é sempre perpendicular ao chão (0, 1, 0) 
    glm::vec3 upVector(0.0f, 1.0f, 0.0f);

    // Posição do "olho" (câmera)
    glm::vec3 eyePosition;

    if (mode == 0) {
        // Modo 1: Olho posicionado no alto da torre 
        // (Assumindo que a torre está na origem (0,0,0) e tem 30 de altura)
        eyePosition = glm::vec3(0.0f, 30.0f, 0.1f); // 0.1f para não ficar 100% reto
    }
    else if (mode == 1) {
        // Modo 2: Atrás do bando, a uma distância fixa 
        // Vamos ficar 20 unidades "atrás" do centro do bando
        // (Assumindo que o bando voa na direção -Z)
        eyePosition = flock.center + glm::vec3(0.0f, 5.0f, 20.0f);
    }
    else { // mode == 2
        // Modo 3: Perpendicular ao vetor velocidade, paralelo ao chão 
        // (Este é o mais complexo, vamos simplificar)
        // Vamos ficar "ao lado" do bando, no eixo X
        eyePosition = flock.center + glm::vec3(25.0f, 3.0f, 0.0f);
    }


    gluLookAt(
        eyePosition.x, eyePosition.y, eyePosition.z, // Posição do "olho"
        lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, // Ponto para onde a câmera olha
        upVector.x,    upVector.y,    upVector.z     // Vetor "para cima"
    );
}