//
// Created by baidu on 2019/2/11.
//

#include "Scene.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

// debug
//#include "loaders.h"
//#include "loader/stb_image.h"

void Scene::addShadow(Light *light) {
    if (light->getLightType() == 0) {
        GLuint texId, fbo;

        // texture
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);


        // debug TODO
//        int width, height, nrChannels;
//        unsigned char* textureData = stbi_load("gradient.jpg", &width, &height, &nrChannels, STBI_rgb);
//        if (!textureData) {
//            std::cout << "Failed to load texture: " << std::endl;
//        }

//        GLuint x = 4;
        for (GLuint i = 0; i < 6; i++) {
//            if (i == x) {
//                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
//            } else {
//                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
//            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            // texture param
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
//        GLuint x = 4;
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGB, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
//        stbi_image_free(textureData);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // frame
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);
        glDrawBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);

        // trans
        GLfloat aspect = (GLfloat)SHADOW_WIDTH/(GLfloat)SHADOW_HEIGHT;
        GLfloat near = 1.0f;
        GLfloat far = 200.0f;

        glm::vec3 lightPos = ((PointLight*)light)->pos;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
        std::vector<glm::mat4> trans = {
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,1.0,0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,1.0,0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,1.0,0.0)),
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,1.0,0.0))
        };

        shadows.push_back({.tex = texId, .light = light, .fbo = fbo, .trans = trans});
    }
}



void Scene::draw() {

    this->current_time = ((float)clock()) / CLOCKS_PER_SEC;

    global_shaders::skybox_shader->scene_phase(this);
//    global_shaders::point_light_shadow_shader->scene_phase(this);
    global_shaders::pbr_shader->scene_phase(this);
//    dynamic_cast<CubeTexDebugShader*>(global_shaders::cube_tex_debug_shader)->debug(this->skybox, 0);
//    ((CubeTexDebugShader*)Shaders::getCubeTexDebugShader())->test(5);
//    ((DebugShader*)Shaders::getDebugShader())->debug(this->textures[0]);
}