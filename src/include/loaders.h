//
// Created by baidu on 2019/1/23.
//

#include <glad/glad.h>
#include <string>
#include "Scene.h"

#ifndef STELLAR_LOADERS_H
#define STELLAR_LOADERS_H

GLuint loadTextureFromFile(std::string path);
GLuint loadCubeTextureFromFile(std::string path);

Scene* loadSceneFromFile(std::string path, Scene* scene = nullptr);

void bindTexture(std::string path, GLenum target = GL_TEXTURE_2D);

#endif //STELLAR_LOADERS_H
