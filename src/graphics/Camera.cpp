// src/graphics/Camera.cpp
#include "graphics/Camera.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Para gluLookAt
#include <glm/gtc/matrix_transform.hpp> // Para glm::lookAt

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
}

void Camera::look(const Flock& flock) {
    // Reseta a matriz de visão do modelo
    glLoadIdentity();

    // TODO: Implementar a lógica dos 3 modos de câmera aqui
    // Por enquanto, apenas uma câmera estática olhando para a origem
    
    // gluLookAt(eyeX, eyeY, eyeZ,  centerX, centerY, centerZ,  upX, upY, upZ)
    gluLookAt(
        0.0, 20.0, 30.0,   // Posição do "olho" (câmera)
        0.0, 0.0, 0.0,    // Ponto para onde a câmera olha (origem)
        0.0, 1.0, 0.0     // Vetor "para cima" (eixo Y)
    );
}