#include "Shaders.h"
#include "Scene.h"

void PointLightShadowShader::init() {
    this->use();
    this->setFloat("far_plane", 200.f);
}

void PointLightShadowShader::scene_phase(Scene *scene) {
    this->use();
    for (Scene::shadow_map &shadow : scene->shadows) {
        Light* light = shadow.light;
        GLuint fbo = shadow.fbo;
        if (light->getLightType() == 0) {
            for (int i = 0; i < 6; i++) {
                this->setMat4(std::string("trans[") + (char)(i + 48) + ']', shadow.trans[i]);
            }
            this->setVec3("light_pos", ((PointLight*)light)->pos);
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glClearDepth(1);
            glClear(GL_DEPTH_BUFFER_BIT);

            scene->drawGroups(this);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
}