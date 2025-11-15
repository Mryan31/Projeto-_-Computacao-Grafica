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
-   I / J / K / L: Move o Boid-Objetivo (líder vermelho) no plano.
-   U / O: Move o Boid-Objetivo para cima e para baixo.
-   + (ou =): Adiciona um novo boid ao bando.
-   -: Remove um boid do bando.
