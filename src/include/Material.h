//
// Created by baidu on 2019/2/7.
//

#include <glm/glm.hpp>
#include <memory>

#ifndef STELLAR_MATERIAL_H
#define STELLAR_MATERIAL_H

class Material {
public:
    enum type{VOID, TEXTURE, COLOR};
    struct _tc{
        type t;
        unsigned int tex;
        glm::vec3 color;
    } tex_color[10];
    float k[10];
    Material() {
        for (int i = 0; i < 10; i++) {
            k[i] = 0;
            tex_color[i].t = type::VOID;
        }
    }
    void insertTex(int i, unsigned int tex) {
        this->tex_color[i] = {type::TEXTURE, tex};
    }
    void insertColor(int i, glm::vec3 color) {
        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &color);
        glBindTexture(GL_TEXTURE_2D, 0);
        this->tex_color[i] = {type::COLOR, tex, color};
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
    ~Material() {
        for (_tc &a : tex_color) {
            if (a.t != type::VOID) {
                glDeleteTextures(1, &(a.tex));
            }
        }
    }
private:
    int color_tex = -1;
};


#endif //STELLAR_MATERIAL_H
