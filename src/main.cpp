// src/main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Para glm::perspective
#include <iostream>
#include <iomanip>

// Nossas classes do projeto
#include "sim/Flock.hpp"
#include "graphics/Camera.hpp"
#include "graphics/World.hpp"
#include "graphics/Renderer.hpp"

// Variáveis globais para controle de input
bool cKeyPressed = false;
bool plusKeyPressed = false;
bool minusKeyPressed = false;
bool sKeyPressed = false;
bool fKeyPressed = false;

bool shadowsEnabled = true;
bool fogEnabled = false;

// Modo de pausa e debug
bool isPaused = false;
bool debugMode = false;
bool pKeyPressed = false;
bool dKeyPressed = false;
bool stepKeyPressed = false;
int debugFrameCount = 0;

// Função para imprimir informações de debug
void printDebugInfo(const Flock& flock, int frame) {
    std::cout << "\n-----------------------------------" << std::endl;
    std::cout << "│  DEBUG - Frame " << frame << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "│ Boids: " << flock.boids.size() << std::endl;
    
    if (!flock.boids.empty()) {
        const Boid& b = flock.boids[0];
        std::cout << "│ Boid[0] Pos: (" 
                  << std::fixed << std::setprecision(2)
                  << b.position.x << ", " 
                  << b.position.y << ", " 
                  << b.position.z << ")" << std::endl;
        
        float speed = glm::length(b.velocity);
        std::cout << "│ Boid[0] Speed: " << speed << std::endl;
    }
    
    glm::vec3 goal = flock.getGoalPosition();
    std::cout << "│ Goal: (" 
              << std::fixed << std::setprecision(2)
              << goal.x << ", " 
              << goal.y << ", " 
              << goal.z << ")" << std::endl;
    
    glm::vec3 center = flock.center;
    std::cout << "│ Center: (" 
              << center.x << ", " 
              << center.y << ", " 
              << center.z << ")" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

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

// Callback para redimensionamento da janela (Reshape)
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Atualiza a projeção quando a janela é redimensionada
    setupProjection(window);
    std::cout << "Janela redimensionada para: " << width << "x" << height << std::endl;
}

// Função para configurar o Fog (névoa)
void setupFog() {
    GLfloat fogColor[] = {0.3f, 0.3f, 0.5f, 1.0f};
    
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 15.0f);
    glFogf(GL_FOG_END, 60.0f);
    glHint(GL_FOG_HINT, GL_NICEST);
}

int main() {
    // ---- 1. Inicialização do GLFW ----
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

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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

    setupFog();

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

        // ---- Processamento de Input ----
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Troca o modo da câmera ao pressionar "C"
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
            camera.nextMode();
            cKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            cKeyPressed = false;
        }

        // Toggle de sombras ao pressionar "S"
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !sKeyPressed) {
            shadowsEnabled = !shadowsEnabled;
            sKeyPressed = true;
            std::cout << "Sombras: " << (shadowsEnabled ? "ATIVADAS" : "DESATIVADAS") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
            sKeyPressed = false;
        }

        // Toggle de fog ao pressionar "F"
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed) {
            fogEnabled = !fogEnabled;
            fKeyPressed = true;
            
            if (fogEnabled) {
                glEnable(GL_FOG);
                std::cout << "Fog: ATIVADO" << std::endl;
            } else {
                glDisable(GL_FOG);
                std::cout << "Fog: DESATIVADO" << std::endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
            fKeyPressed = false;
        }

        // ===== MODO PAUSA E DEBUG =====
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pKeyPressed) {
            isPaused = !isPaused;
            pKeyPressed = true;
            
            if (isPaused) {
                std::cout << "\n PAUSADO - Pressione P para continuar" << std::endl;
            } else {
                std::cout << "RODANDO" << std::endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            pKeyPressed = false;
        }

        // Toggle de modo debug com 'D'
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !dKeyPressed) {
            debugMode = !debugMode;
            dKeyPressed = true;
            
            if (debugMode) {
                isPaused = true;
                debugFrameCount = 0;
                std::cout << "\n-----------------------------------" << std::endl;
                std::cout << "||   MODO DEBUG ATIVADO           ||" << std::endl;
                std::cout << "||   SPACE = Avancar 1 frame      ||" << std::endl;
                std::cout << "||   D = Desativar debug          ||" << std::endl;
                std::cout << "-----------------------------------" << std::endl;
            } else {
                std::cout << "\n[DEBUG DESATIVADO]" << std::endl;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
            dKeyPressed = false;
        }

        // Passo-a-passo no modo debug
        if (debugMode && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !stepKeyPressed) {
            stepKeyPressed = true;
            debugFrameCount++;
            
            flock.update(0.016f);
            printDebugInfo(flock, debugFrameCount);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            stepKeyPressed = false;
        }

        // Controle do Boid-Objetivo (Líder)
        {
            glm::vec3 goalVel(0.0f);
            float leaderSpeed = 5.0f; // 5 unidades/segundo

            if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // Frente
                goalVel.z = -leaderSpeed;
            if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Trás
                goalVel.z = leaderSpeed;
            if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Esquerda
                goalVel.x = -leaderSpeed;
            if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Direita
                goalVel.x = leaderSpeed;
            if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Cima
                goalVel.y = leaderSpeed;
            if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // Baixo
                goalVel.y = -leaderSpeed;
            
            flock.setGoalVelocity(goalVel);
        }

        // Adicionar/Remover Boids
        // Tecla '+' (que é a mesma que '=')
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && !plusKeyPressed) {
            flock.addBoid();
            plusKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE) {
            plusKeyPressed = false;
        }
        // Tecla '-'
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && !minusKeyPressed) {
            flock.removeBoid();
            minusKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE) {
            minusKeyPressed = false;
        }

        // ---- Lógica/Update ----
        // Só atualiza se não estiver pausado E não estiver em modo debug
        // No modo debug, o update é feito manualmente com SPACE
        if (!isPaused && !debugMode) {
            flock.update(deltaTime); // Atualiza a posição dos boids
        }

        // ---- Renderização ----
        // Limpa a tela
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // Fundo azul escuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configura a projeção 3D (importante para redimensionar a janela)
        setupProjection(window);

        // 1. Configura a câmera
        camera.look(flock);

        // 2. Desenha o cenário (chão, torre)
        world.draw();

        // 3. Desenha as sombras dos boids
        if (shadowsEnabled) {
            glm::vec3 lightDir(50.0f, 50.0f, 100.0f);
            renderer.drawShadows(flock, lightDir);
        }

        // 4. Desenha os boids
        renderer.draw(flock);

        // ---- Troca os Buffers e Processa Eventos ----
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---- 5. Limpeza ----
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}