#ifndef SHADOW_HPP
#define SHADOW_HPP

#include <glm/glm.hpp>

class Shadow {
public:
    static glm::mat4 getShadowMatrix(const glm::vec3& lightDir, float planeY = 0.0f);
    
    static void beginShadowRender(float alpha = 0.4f);
    
    static void endShadowRender();
};

#endif
