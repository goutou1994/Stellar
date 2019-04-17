#include "Shaders.h"
#include "Scene.h"
#include "loaders.h"

#define PBR_TEST_METALLIC .7f
#define PBR_TEST_ROUGHNESS .2f

void PBRShader::init() {
    this->use();
    this->setInt("diffuseIBL", 11);
    this->setInt("specIBL", 12);
    this->setInt("brdfLUT", 13);
    this->lut = loadTextureFromFile("textures/ibl_brdf_lut.png");
}

void PBRShader::scene_phase(Scene *scene) {
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene->skybox[1]);
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene->skybox[2]);
    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, this->lut);
    glActiveTexture(0);
    TestShader::scene_phase(scene);
}