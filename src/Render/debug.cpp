#include "Shaders.h"

void DebugShader::init() {
    GLuint VBO;
    std::cout << "debug init" << std::endl;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);
}

void DebugShader::debug(GLuint tex) {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    this->use();
    this->setInt("tex", 0);

    glViewport(0, 0, 500 * 2, 300 * 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glDrawArrays(GL_POINTS, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}