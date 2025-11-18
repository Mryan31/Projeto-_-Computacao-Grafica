Trabalho Prático - Boids (UFMG)

Este projeto é uma implementação do algoritmo de Boids (simulação de
bando) para a disciplina de Computação Gráfica. Ele usa C++, OpenGL
(modo de compatibilidade), GLFW, GLAD e GLM.

------------------------------------------------------------------------

Dependencies

Todas as dependências necessárias (GLFW, GLAD, GLM) estão incluídas
(“vendored”) na pasta /lib deste repositório.

-   GLFW (64-bit)
-   GLAD
-   GLM

Como Compilar e Executar

Este projeto usa make e o compilador g++ (MinGW 64-bit).

1. Compilando

No terminal, na raiz do projeto, execute:

    make clean
    make

Isso criará um executável chamado boids_app.exe.

2. Executando

Após a compilação, execute:

    ./boids_app.exe

Controles

-   ESC: Fecha a aplicação.
-   C: Alterna entre os 3 modos de câmera (Torre, Atrás do Bando,
    Lateral).
-   S: Liga/Desliga as sombras dos boids no chão.
-   F: Liga/Desliga o fog (névoa/neblina).
-   P: Pausa/Continua a simulação.
-   D: Ativa/Desativa modo debug (passo-a-passo).
-   SPACE: (Modo debug) Avança 1 frame por vez.
-   I / J / K / L: Move o Boid-Objetivo (líder vermelho) no plano.
-   U / O: Move o Boid-Objetivo para cima e para baixo.
-   + (ou =): Adiciona um novo boid ao bando.
-   -: Remove um boid do bando.

Funcionalidades

Funcionalidades Básicas (80%)

-   ✅ Mundo 3D com chão e torre central em forma de cone
-   ✅ 3 modos de câmera (Torre, Atrás do Bando, Lateral)
-   ✅ Iluminação OpenGL com luz ambiente e direcional
-   ✅ Boids desenhados como poliedros 3D
-   ✅ Adicionar/Remover boids dinamicamente
-   ✅ Animação de batida de asas

Funcionalidades Extras Implementadas (20%)

-   ✅ Sombras (5%): Projeção planar dos boids no chão com toggle
-   ✅ Reshape (5%): Redimensionamento dinâmico da janela mantendo
    proporção correta
-   ✅ Fog (5%): Efeito de névoa/neblina que aumenta com a distância
-   ✅ Modo Pausa (5%): Sistema completo de pausa e debug
    -   Pausa/continua com tecla P
    -   Adicionar/remover boids durante pausa
    -   Modo debug passo-a-passo (D + SPACE)
    -   Informações detalhadas no console