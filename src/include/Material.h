//
// Created by baidu on 2019/2/7.
//

#include <glm/glm.hpp>
#include <memory>

#ifndef STELLAR_MATERIAL_H
#define STELLAR_MATERIAL_H

class Material {
public:
    enum type{TEXTURE, COLOR, VOID};
    struct {
        type t;
        union {
            unsigned int tex;
            glm::vec3 color;
        };
    } tex_color[10];
    float k[10];
    Material() {
        for (int i = 0; i < 10; i++) {
            k[i] = 0;
            tex_color[i].t = type::VOID;
        }
    }
    type getType(int i) {
        return tex_color[i].t;
    }
    GLuint tex(int i) {
        return tex_color[i].tex;
    }
    glm::vec3 color(int i) {
        return tex_color[i].color;
    }
};


#endif //STELLAR_MATERIAL_H
