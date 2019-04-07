#include "Shaders.h"

#define PBR_TEST_METALLIC 1.f
#define PBR_TEST_ROUGHNESS .1f

void PBRShader::init() {
    this->use();
    this->setFloat("metallic", PBR_TEST_METALLIC);
    this->setFloat("roughness", PBR_TEST_ROUGHNESS);
}

void PBRShader::scene_phase(Scene *scene) {
    TestShader::scene_phase(scene);
}