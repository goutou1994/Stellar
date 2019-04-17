#include "Shaders.h"

void EquirectangularShader::init() {
    CubeTexShader::init();
    this->use();
    this->setInt("tex2D", 0);
}

unsigned int EquirectangularShader::convert(unsigned int tex2d, int width) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex2d);
    return CubeTexShader::draw(width);
}