#include "Shaders.h"
#include <cmath>

void SpecIBLMapShader::init() {
    CubeTexShader::init();
    this->use();
    this->setInt("texCube", 0);
}

unsigned int SpecIBLMapShader::convert(unsigned int texCubeOrigin, int width) {
    this->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texCubeOrigin);
    glActiveTexture(GL_TEXTURE1);

    unsigned int texCube;
    glGenTextures(1, &texCube);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, width, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    const int maxMipLevel = 5;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, maxMipLevel);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindVertexArray(VAO);

    for (int mip = 0; mip < maxMipLevel; mip++) {
        glViewport(0, 0, width * std::pow(.5f, mip), width * std::pow(.5f, mip));
        this->setFloat("roughness", (float)mip / (float)(maxMipLevel - 1));
        for (int i = 0; i < 6; i++) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texCube, mip);
            this->setMat4("trans", this->trans[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return texCube;
}