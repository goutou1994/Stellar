//
// Created by baidu on 2019/2/4.
//
#include <glm/glm.hpp>
#include <set>
#include "Model.h"
#include "Shaders.h"
#include "Transform.h"
#include "Material.h"

#include <iostream> //debug

#ifndef STELLAR_GROUP_H
#define STELLAR_GROUP_H

class Group {
public:
    virtual bool is_model() = 0;
    virtual void draw(Shader* shader, float time = .0f, glm::mat4 parent_trans = glm::mat4(1.0f)) = 0;
    Transform transform;
    Group* parent = nullptr;
    virtual ~Group() {}
};

class GroupNode : public Group {
public:
    bool is_model() {
        return false;
    }
    std::set<Group*> children;
    void removeChild(Group*);
    void appendChild(Group*);
    void draw(Shader* shader, float time = .0f, glm::mat4 parent_trans = glm::mat4(1.0f));
};

class ModelNode : public Group {
public:
    static ModelNode* create(Model* model) {
        return new ModelNode(model);
    }
    bool is_model() {
        return true;
    }
    Model* model;
    unsigned int textures[8];
    Material *mtl;
    void draw(Shader* shader, float time = .0f, glm::mat4 parent_trans = glm::mat4(1.0f));
    ~ModelNode() {
        delete model;
    }
private:
    ModelNode(Model* model) : model(model) {
        for (unsigned int &tex : textures) {
            tex = -1;
        }
    }
};

#endif //STELLAR_GROUP_H
