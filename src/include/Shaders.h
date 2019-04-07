//
// Created by baidu on 2019/2/7.
//

#include "Shader.h"

#ifndef STELLAR_SHADERS_H
#define STELLAR_SHADERS_H

namespace global_shaders {
    extern Shader *normal_shader;
    extern Shader *point_light_shadow_shader;
    extern Shader *debug_shader;
    extern Shader *cube_tex_debug_shader;
    extern Shader *pbr_shader;
    extern Shader *current_shader;
    extern Shader *skybox_shader;

    void init();
}

class NormalShader : public Shader {
public:
    NormalShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
        : Shader(vertexPath, fragmentPath, geometryPath) {}
    void scene_phase(Scene*);
};

class PointLightShadowShader : public Shader {
public:
    PointLightShadowShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    void scene_phase(Scene*);
};

class TestShader : public Shader {
public:
    TestShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    void scene_phase(Scene*);
};

class DebugShader : public Shader {
public:
    DebugShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    void debug(GLuint tex);
private:
    GLuint VAO;
};

class CubeTexDebugShader : public Shader {
public:
    CubeTexDebugShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    void test(GLuint face = 0);
    void debug(GLuint tex, GLuint face);
private:
    GLuint VAO, testTex;
    glm::mat4 trans[6];
};

class PBRShader : public TestShader {
public:
    PBRShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : TestShader(vertexPath, fragmentPath, geometryPath) {
        init();
    }

    void init();
    void scene_phase(Scene*) override;
};

class SkyboxShader : public Shader {
public:
    SkyboxShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }

    void init();
    void scene_phase(Scene*) override;
private:
    GLuint VAO;
};

#endif //STELLAR_SHADERS_H
