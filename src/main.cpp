// src/main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Para glm::perspective
#include <iostream>

// Nossas classes do projeto
#include "sim/Flock.hpp"
#include "graphics/Camera.hpp"
#include "graphics/World.hpp"
#include "graphics/Renderer.hpp"

// Função para configurar a projeção 3D
void setupProjection(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (height == 0) height = 1; // Evita divisão por zero

    float aspectRatio = (float)width / (float)height;

    glViewport(0, 0, width, height);
    
    // Define a matriz de projeção (Perspectiva)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Usando GLM para criar a matriz de perspectiva (substitui o gluPerspective)
    // Campo de visão de 45 graus, aspect ratio da tela, ver de 0.1 a 1000 unidades
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
    glLoadMatrixf(&projectionMatrix[0][0]);

    // Volta para a matriz de ModelView
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main() {
    // ---- 1. Inicialização do GLFW ----
    bool cKeyPressed = false;

    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return -1;
    }

    // Configura a versão do OpenGL (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // *** MUDANÇA IMPORTANTE: Usando o perfil "Compatibility" ***
    // Isso nos dá acesso às funções fáceis (glBegin, glTranslatef, etc.)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // ---- 2. Criação da Janela ----
    GLFWwindow* window = glfwCreateWindow(800, 600, "Trabalho Boids - UFMG", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ---- 3. Inicialização do GLAD ----
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD!" << std::endl;
        return -1;
    }

    // ---- Configurações do OpenGL ----
    glEnable(GL_DEPTH_TEST); // Essencial para 3D
    glEnable(GL_LIGHTING);   // Habilita luz (requisito)
    glEnable(GL_LIGHT0);     // Habilita luz 0
    glEnable(GL_COLOR_MATERIAL); // Permite que glColor() afete o material

    // Define uma luz ambiente e uma luz direcional simples
    float ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float lightPosition[] = { 50.0f, 50.0f, 100.0f, 0.0f }; // Luz direcional "do céu"
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    // ---- Nossos Objetos Principais ----
    Flock flock;
    Camera camera;
    World world;
    Renderer renderer;

    float lastTime = (float)glfwGetTime();

    // ---- 4. O Loop Principal (Game Loop) ----
    while (!glfwWindowShouldClose(window)) {
        // --- Cálculo do DeltaTime ---
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // ---- Processamento de Input (Exemplo) ----
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
            camera.nextMode();
            cKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            cKeyPressed = false;
        }
        // TODO: Adicionar inputs para câmera e boids

        // ---- Lógica/Update ----
        flock.update(deltaTime); // Atualiza a posição dos boids

        // ---- Renderização ----
        // Limpa a tela
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // Fundo azul escuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configura a projeção 3D (importante para redimensionar a janela)
        setupProjection(window);

        // 1. Configura a câmera
        camera.look(flock); // Pessoa A implementa isso

        // 2. Desenha o cenário (chão, torre)
        world.draw(); // Pessoa A implementa isso

        // 3. Desenha os boids
        renderer.draw(flock); // Pessoa A implementa isso

        // ---- Troca os Buffers e Processa Eventos ----
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---- 5. Limpeza ----
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}