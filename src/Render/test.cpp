#include "Shaders.h"
#include "Scene.h"

void TestShader::init() {
    this->use();
    for (int i = 0; i < 8; i++) {
        this->setInt(std::string("tex") + (char)(i + 48), i);
    }
    this->setInt("shadowTex", 8);
    this->setInt("shadowIndex", -1);
}

void TestShader::scene_phase(Scene *scene) {
    this->use();

    glm::mat4 trans = scene->camera->getTransMat();
    this->setMat4("trans", trans);
//    glm::vec3 eye_pos = scene->camera->getCameraPosition();
//    std::cout << eye_pos.x << ' ' << eye_pos.y << ' ' << eye_pos.z << std::endl;
    this->setVec3("eye_pos", scene->camera->getCameraPosition());

    Light* shadow_light = nullptr;

    for (Scene::shadow_map &shadow : scene->shadows) {
        shadow_light = shadow.light;
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadow.tex);
        break;
    }

    int point_light_num = 0;
    for (Light* light : scene->lights) {
        if (light == shadow_light) {
            this->setInt("shadowIndex", point_light_num);
        }
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
    glClear(GL_DEPTH_BUFFER_BIT);

    scene->drawGroups(this);
}