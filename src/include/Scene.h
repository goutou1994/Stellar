//
// Created by baidu on 2019/1/22.
//

#include "Group.h"
#include "Light.h"
#include <map>
#include "Shader.h"
#include "Material.h"
#include "Animator.h"
#include "Camera.h"

#ifndef STELLAR_SCENE_H
#define STELLAR_SCENE_H

const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class Scene {
public:
    std::vector<Group*> groups;
    std::map<std::string, Group*> group_map;
    GroupNode* group_root = new GroupNode();
    std::vector<unsigned int> textures;
    std::map<std::string, unsigned int> tex_map;
    std::vector<Light*> lights;
    std::map<std::string, Light*> light_map;
    std::vector<Material*> materials;
    std::map<std::string, Material*> material_map;
    std::vector<BaseAnimator<float>*> animators;
    std::map<std::string, BaseAnimator<float>*> animator_map;
    BaseCamera* camera;
    int skybox[3] = {-1, -1, -1};

    struct shadow_map{
        Light* light;
        unsigned int tex, fbo;
        std::vector<glm::mat4> trans;
    };
    std::vector<shadow_map> shadows;

    float origin_time = 0, last_time, current_time;

    void addShadow(Light*);
    void addSkybox(unsigned int skybox);
    void drawGroups(Shader* shader) {
        this->group_root->draw(shader, this->current_time);
    }
    void draw();
    ~Scene() {
        delete group_root;
    }
    friend Shader;
};

#endif //STELLAR_SCENE_H
