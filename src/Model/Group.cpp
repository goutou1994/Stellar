#include "Group.h"

void GroupNode::appendChild(Group *child) {
    if (child->parent != nullptr) {
        dynamic_cast<GroupNode*>(child->parent)->removeChild(child);
    }
    this->children.insert(child);
    child->parent = this;
}

void GroupNode::removeChild(Group *child) {
    auto i = this->children.find(child);
    if (i != this->children.end()) {
        this->children.erase(i);
        child->parent = nullptr;
    }
}

void GroupNode::draw(Shader* shader, float time, glm::mat4 parent_trans) {
    glm::mat4 group_trans = parent_trans * this->transform.getTransMat(time);
    for (Group *child : this->children) {
        child->draw(shader, time, group_trans);
    }
}

void ModelNode::draw(Shader* shader, float time, glm::mat4 parent_trans) {
    shader->use();
    glm::mat4 model_trans = this->transform.getTransMat();
    shader->setMat4("model_trans", parent_trans * this->transform.getTransMat(time));

    int active_textures = 0;
    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (mtl->getType(i) == Material::TEXTURE) {
            active_textures |= 1 << i;
            glBindTexture(GL_TEXTURE_2D, mtl->tex(i));
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    shader->setInt("active_textures", active_textures);
    model->draw();
}