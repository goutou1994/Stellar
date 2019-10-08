//
// Created by baidu on 2019/2/8.
//

#include <glm/glm.hpp>

#ifndef STELLAR_LIGHT_H
#define STELLAR_LIGHT_H

class Light {
public:
    Light(const glm::vec3 &color, float I) : color(color), I(I) {}
    glm::vec3 color;
    float I;
    virtual int getLightType() = 0;
};

class PointLight : public Light {
public:
    PointLight(const glm::vec3 &color, float I, const glm::vec3 &pos) : Light(color, I), pos(pos) {}
    glm::vec3 pos;
    int getLightType() {
        return 0;
    }
};

class Ambient : public Light {
public:
    Ambient(const glm::vec3 &color, float I) : Light(color, I) {}
    int getLightType() {
        return 3;
    }
};

class Directional: public Light {
public:
    Directional(const glm::vec3 &color, float I, const glm::vec3 &dir) : Light(color, I), dir(dir) {}
    glm::vec3 dir;
    int getLightType() {
        return 1;
    }
};

class SpotLight: public Light {
public:
    SpotLight(const glm::vec3 &color, float I, const glm::vec3 &pos, const glm::vec3 &lookat, float fov)
            : Light(color, I), pos(pos), lookat(lookat) {
        this->fov = glm::radians(fov);
    }
    glm::vec3 pos, lookat;
    float fov;
    int getLightType() {
        return 2;
    }
};

#endif //STELLAR_LIGHT_H
