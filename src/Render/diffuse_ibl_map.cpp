#include "Shaders.h"

void DiffuseIBLMapShader::init() {
    CubeTexShader::init();
    this->use();
    this->setInt("texCube", 0);
}

unsigned int DiffuseIBLMapShader::convert(unsigned int texCube, int width) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
    glActiveTexture(GL_TEXTURE1);
    return CubeTexShader::draw(width);
}