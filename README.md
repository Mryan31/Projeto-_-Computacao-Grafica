# Trabalho Prático - Boids (Computação Gráfica - UFMG)

[cite_start]Este projeto implementa uma simulação de bando ("flock") de pássaros virtuais (boids) em um espaço 3D, baseado no trabalho prático proposto por Renato Ferreira[cite: 2, 3].

[cite_start]O bando obedece a três regras principais: Separação [cite: 8][cite_start], Coesão [cite: 9] [cite_start]e Alinhamento[cite: 10].

## 👥 Autores

* Mateus Ryan de Castro Lima
* [Nome do Integrante 2]

## 🚀 Como Executar

1.  **Clone o repositório:**
    ```bash
    git clone [URL_DO_SEU_REPOSITORIO]
    cd trabalho-boids-ufmg
    ```

2.  **Crie e ative um ambiente virtual:**
    ```bash
    # Windows
    python -m venv venv
    .\venv\Scripts\activate

    # macOS/Linux
    python3 -m venv venv
    source venv/bin/activate
    ```

3.  **Instale as dependências:**
    ```bash
    pip install -r requirements.txt
    ```

4.  **Execute o programa:**
    ```bash
    python src/main.py
    ```

## 🎮 Controles

* [cite_start]**Setas (ou WASD):** Controlam o boid-objetivo[cite: 13].
* [cite_start]**Tecla `+`:** Adiciona um novo boid[cite: 24].
* [cite_start]**Tecla `-`:** Remove um boid[cite: 24].
* [cite_start]**Tecla `C` (ou 1, 2, 3):** Alterna os modos de câmera[cite: 18].
* [cite_start]**Tecla `P`:** Pausa/despausa a simulação[cite: 34].
* [cite_start]**Tecla `F`:** Ativa/desativa o fog[cite: 33].