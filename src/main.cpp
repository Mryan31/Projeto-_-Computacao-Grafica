#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iomanip>

#include "sim/Flock.hpp"
#include "graphics/Camera.hpp"
#include "graphics/World.hpp"
#include "graphics/Renderer.hpp"

bool cKeyPressed = false;
bool plusKeyPressed = false;
bool minusKeyPressed = false;
bool sKeyPressed = false;
bool fKeyPressed = false;

bool shadowsEnabled = true;
bool fogEnabled = false;

bool isPaused = false;
bool debugMode = false;
bool pKeyPressed = false;
bool dKeyPressed = false;
bool stepKeyPressed = false;
int debugFrameCount = 0;

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

void setupProjection(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (height == 0) height = 1; // Evita divisão por zero

    float aspectRatio = (float)width / (float)height;

    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
    glLoadMatrixf(&projectionMatrix[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    setupProjection(window);
    std::cout << "Janela redimensionada para: " << width << "x" << height << std::endl;
}

void setupFog() {
    GLfloat fogColor[] = {0.3f, 0.3f, 0.5f, 1.0f};
    
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 15.0f);
    glFogf(GL_FOG_END, 60.0f);
    glHint(GL_FOG_HINT, GL_NICEST);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Trabalho Boids - UFMG", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Essencial para 3D
    glEnable(GL_LIGHTING);   // Habilita luz (requisito)
    glEnable(GL_LIGHT0);     // Habilita luz 0
    glEnable(GL_COLOR_MATERIAL); // Permite que glColor() afete o material

    float ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float lightPosition[] = { 50.0f, 50.0f, 100.0f, 0.0f }; // Luz direcional "do céu"
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    setupFog();

    Flock flock;
    Camera camera;
    World world;
    Renderer renderer;

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cKeyPressed) {
            camera.nextMode();
            cKeyPressed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            cKeyPressed = false;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !sKeyPressed) {
            shadowsEnabled = !shadowsEnabled;
            sKeyPressed = true;
            std::cout << "Sombras: " << (shadowsEnabled ? "ATIVADAS" : "DESATIVADAS") << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
            sKeyPressed = false;
        }

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

        if (debugMode && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !stepKeyPressed) {
            stepKeyPressed = true;
            debugFrameCount++;
            
            flock.update(0.016f);
            printDebugInfo(flock, debugFrameCount);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
            stepKeyPressed = false;
        }

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

        if (!isPaused && !debugMode) {
            flock.update(deltaTime); // Atualiza a posição dos boids
        }

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // Fundo azul escuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupProjection(window);

        camera.look(flock);

        world.draw();

        if (shadowsEnabled) {
            glm::vec3 lightDir(50.0f, 50.0f, 100.0f);
            renderer.drawShadows(flock, lightDir);
        }

        renderer.draw(flock);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}