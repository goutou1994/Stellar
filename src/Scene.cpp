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
    GLuint texId, fbo;

    std::vector<glm::mat4> trans;

    if (light->getLightType() == 0) {
        if (light->getLightType() == 0) {
            // texture
            glGenTextures(1, &texId);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

            // texture param
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            for (GLuint i = 0; i < 6; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            }

            // trans
            GLfloat aspect = (GLfloat)SHADOW_WIDTH/(GLfloat)SHADOW_HEIGHT;
            GLfloat near = 1.0f;
            GLfloat far = 200.0f;

            glm::vec3 lightPos = ((PointLight*)light)->pos;
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
            trans = {
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,1.0,0.0)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,1.0,0.0)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,1.0,0.0)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,1.0,0.0))
            };
        }
    } else if (light->getLightType() == 2) {
        SpotLight *spotLight = dynamic_cast<SpotLight*>(light);

        // texture
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        // texture param
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        // trans
        GLfloat aspect = 1.f;
        GLfloat near = .1f;
        GLfloat far = 200.0f;

        glm::vec3 lightPos = spotLight->pos;
        glm::mat4 perspective = glm::perspective(spotLight->fov, aspect, near, far);
        glm::mat4 lookAt = glm::lookAt(spotLight->pos, spotLight->lookat, glm::vec3(0, 1, 0));
        trans.push_back(perspective * lookAt);
    }

    // frame
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texId, 0);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    shadows.push_back({.tex = texId, .light = light, .fbo = fbo, .trans = trans});
}

void Scene::addSkybox(unsigned int skybox) {
    this->skybox[0] = skybox;
    this->skybox[1] = dynamic_cast<DiffuseIBLMapShader*>(global_shaders::diffuse_ibl_map_shader)->convert(skybox, 128);
    this->skybox[2] = dynamic_cast<SpecIBLMapShader*>(global_shaders::spec_ibl_map_shader)->convert(skybox, 128);
}


void Scene::draw() {

    this->current_time = ((float)clock()) / CLOCKS_PER_SEC;

//    global_shaders::skybox_shader->scene_phase(this);
//    global_shaders::point_light_shadow_shader->scene_phase(this);
    global_shaders::spot_light_shadow_shader->scene_phase(this);
    global_shaders::pbr_shader->scene_phase(this);
//    dynamic_cast<CubeTexDebugShader*>(global_shaders::cube_tex_debug_shader)->debug(this->skybox, 0);
//    dynamic_cast<DebugShader*>(global_shaders::debug_shader)->debug(this->shadows[0].tex);
}