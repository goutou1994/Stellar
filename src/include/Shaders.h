//
// Created by baidu on 2019/2/7.
//

#include "Shader.h"

#ifndef STELLAR_SHADERS_H
#define STELLAR_SHADERS_H

namespace global_shaders {
    extern Shader *normal_shader;
    extern Shader *point_light_shadow_shader;
    extern Shader *spot_light_shadow_shader;
    extern Shader *debug_shader;
    extern Shader *cube_tex_debug_shader;
    extern Shader *pbr_shader;
    extern Shader *current_shader;
    extern Shader *skybox_shader;
    extern Shader *equirectangular_shader;
    extern Shader *diffuse_ibl_map_shader;
    extern Shader *spec_ibl_map_shader;

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

class SpotLightShadowShader : public Shader {
public:
    SpotLightShadowShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
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
private:
    unsigned int lut;
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

class CubeTexShader : public Shader {
public:
    CubeTexShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : Shader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    unsigned int draw(int width = 1024);
protected:
    unsigned int FBO, VAO;
    glm::mat4 trans[6];
};

class EquirectangularShader : public CubeTexShader {
public:
    EquirectangularShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : CubeTexShader(vertexPath, fragmentPath, geometryPath) {
        init();
    }

    void init();
    unsigned int convert(unsigned int tex, int width = 1024);
};

class DiffuseIBLMapShader : public CubeTexShader {
public:
    DiffuseIBLMapShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : CubeTexShader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    unsigned int convert(unsigned int texCube, int width = 1024);
};

class SpecIBLMapShader : public CubeTexShader {
public:
    SpecIBLMapShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : CubeTexShader(vertexPath, fragmentPath, geometryPath) {
        init();
    }
    void init();
    unsigned int convert(unsigned int texCube, int width = 1024);
};

#endif //STELLAR_SHADERS_H
