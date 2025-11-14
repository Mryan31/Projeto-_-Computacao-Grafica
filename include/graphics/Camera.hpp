//Classe crucial para implementar os 3 modos de visualização pedidos. Ela vai calcular a matriz view (usando gluLookAt ou matemática de matrizes com GLM).
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "sim/Flock.hpp"

#include <glm/glm.hpp>

class Camera{
public:
    int mode; // 0 = Torre, 1 = Atrás, 2 = Lateral

    Camera();

    // Configura o gluLookAt() baseado no modo e na posição do bando
    void look(const Flock& flock);
    void nextMode();
};

#endif