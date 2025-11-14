// -------------------------------------------------------------------------
// Arquivo: src/main.cpp
// -------------------------------------------------------------------------

// 1. Inclui o GLAD (Deve vir ANTES do GLFW)
//    (Estamos assumindo que você copiou as pastas 'glad/' e 'KHR/' 
//    para sua pasta 'include/')
#include <glad/glad.h>

// 2. Inclui o GLFW
//    (Estamos assumindo que você tem uma pasta 'glfw/' dentro de 'lib/')
#include <GLFW/glfw3.h>

// 3. Inclui o GLM (para matemática)
//    (Estamos assumindo que você tem uma pasta 'glm/' dentro de 'lib/')
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3

// Inclui o iostream padrão do C++ para imprimir no console
#include <iostream>

int main() {
    // ---- 1. Inicialização do GLFW ----
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return -1;
    }

    // Configura a versão do OpenGL (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ---- 2. Criação da Janela ----
    GLFWwindow* window = glfwCreateWindow(800, 600, "Trabalho Boids - UFMG", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ---- 3. Inicialização do GLAD ----
    //    (Carrega as funções modernas do OpenGL)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD!" << std::endl;
        return -1;
    }

    std::cout << "GLFW e GLAD inicializados com sucesso!" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // ---- 4. O Loop Principal (Game Loop) ----
    while (!glfwWindowShouldClose(window)) {
        // ---- Processamento de Input (por enquanto, só o ESC) ----
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // ---- Renderização ----
        // Define a cor de fundo (um azul escuro)
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        // Limpa o buffer de cor
        glClear(GL_COLOR_BUFFER_BIT);

        // ---- Troca os Buffers e Processa Eventos ----
        // (Mostra o que foi desenhado na tela)
        glfwSwapBuffers(window);
        // (Verifica por eventos de teclado/mouse)
        glfwPollEvents();
    }

    // ---- 5. Limpeza ----
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}