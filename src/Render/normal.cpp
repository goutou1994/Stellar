#include "Shaders.h"
#include "Scene.h"

void NormalShader::scene_phase(Scene *scene) {
    this->use();

    // Temporary light initialization
    int point_light_num = 0;
    for (Light* light : scene->lights) {
        if (light->getLightType() == 0) {
            std::string path = "point_lights[" + std::to_string(point_light_num) + "].";
            this->setVec3(path + "pos", ((PointLight*)light)->pos);
            this->setVec3(path + "color", light->color);
            this->setFloat(path + "I", light->I);
            point_light_num++;
        } else if (light->getLightType() == 3) {
            this->setVec3("ambient.color", light->color);
            this->setFloat("ambient.I", light->I);
        }
    }
    this->setInt("point_light_num", point_light_num);
    // end

    glViewport(0, 0, 500 * 2, 300 * 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    scene->group_root->draw(this);
}
