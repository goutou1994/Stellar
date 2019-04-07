#include "Shaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include "loaders.h"

void CubeTexDebugShader::init() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(0);

    trans[0] = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
    trans[1] = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
    trans[2] = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0, 0));
    trans[3] = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0, 0));
    trans[4] = glm::mat4(1.0f);
    trans[5] = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
    this->use();
    for (int i = 0; i < 6; i++) {
        this->setMat4(std::string("trans[") + (char)(i + 48) + ']', trans[i]);
    }

    glGenTextures(1, &testTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, testTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        bindTexture(std::string("textures/pure/pure") + (char)(i + 48) + ".jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
//        bindTexture("gradient.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }
}

void CubeTexDebugShader::test(GLuint face) {
    this->debug(this->testTex, face);
}

void CubeTexDebugShader::debug(GLuint tex, GLuint face) {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    this->use();
    this->setInt("tex", 0);
    this->setInt("face", face);


    glViewport(0, 0, 500 * 2, 300 * 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glDrawArrays(GL_POINTS, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}