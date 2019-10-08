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
        if (shadow_light->getLightType() == 0) {
//            glBindTexture(GL_TEXTURE_CUBE_MAP, shadow.tex);
        } else if (shadow_light->getLightType() == 2) {
            glBindTexture(GL_TEXTURE_2D, shadow.tex);
            this->setMat4("shadow_trans", shadow.trans[0]);
        }
        break;
    }

    int light_count = 0;
    for (Light* light : scene->lights) {
        if (light == shadow_light) {
            this->setInt("shadowIndex", light_count);
        }
        std::string path = "lights[" + std::to_string(light_count) + "].";
        this->setInt(path + "type", light->getLightType());
        this->setVec3(path + "color", light->color);
        this->setFloat(path + "I", light->I);
        if (light->getLightType() == 0) {
            this->setVec3(path + "pos", dynamic_cast<PointLight*>(light)->pos);
        }
        if (light->getLightType() == 1) {
            this->setVec3(path + "dir", dynamic_cast<Directional*>(light)->dir);
        }
        if (light->getLightType() == 2) {
            this->setVec3(path + "dir", glm::normalize(dynamic_cast<SpotLight*>(light)->lookat - dynamic_cast<SpotLight*>(light)->pos));
            this->setVec3(path + "pos", dynamic_cast<SpotLight*>(light)->pos);
            this->setFloat(path + "fov", dynamic_cast<SpotLight*>(light)->fov);
        }
        light_count++;
    }
    this->setInt("light_count", light_count);
    // end

    glViewport(0, 0, 1000 * 2, 600 * 2);
    glClear(GL_DEPTH_BUFFER_BIT);

    scene->drawGroups(this);
}