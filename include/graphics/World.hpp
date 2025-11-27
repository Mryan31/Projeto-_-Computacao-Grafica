//Terá funções para desenhar o chão (plano) e a torre (cone)  usando chamadas do OpenGL.
#ifndef WORLD_HPP
#define WORLD_HPP

struct GLUquadric;

class World{
public:
    World();
    ~World();

    //Desenha o chão
    void draw();
    
    //Desenha o sol
    void drawSun();

private:
    GLUquadric* quadric;
};

#endif