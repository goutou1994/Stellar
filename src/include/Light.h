//
// Created by baidu on 2019/2/8.
//

#include <glm/glm.hpp>

#ifndef STELLAR_LIGHT_H
#define STELLAR_LIGHT_H

class Light {
public:
    Light(float r, float g, float b, float I) : color(r, g, b), I(I) {}
    glm::vec3 color;
    float I;
    virtual int getLightType() = 0;
};

class PointLight : public Light {
public:
    PointLight(float r, float g, float b, float I, float x, float y, float z) : Light(r, g, b, I), pos(x, y, z) {}
    glm::vec3 pos;
    int getLightType() {
        return 0;
    }
};

class Ambient : public Light {
public:
    Ambient(float r, float g, float b, float I) : Light(r, g, b, I) {}
    int getLightType() {
        return 3;
    }
};
#endif //STELLAR_LIGHT_H
