//
// Created by baidu on 2019/3/7.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Animator.h"

#ifndef STELLAR_TRANSFORM_H
#define STELLAR_TRANSFORM_H

class Transform {
public:
    Transform() {
        refresh = true;
    }
    void setScale(glm::vec3 &&scale) {
        this->scale = scale;
        refresh = true;
    }
    void setScale(float scale) {
        this->scale = glm::vec3(scale);
        refresh = true;
    }
    void setRotation(glm::vec3 &&rotation) {
        this->rotation = rotation;
        refresh = true;
    }
    void setPos(glm::vec3 &&pos) {
        this->pos = pos;
        refresh = true;
    }
    void setPosX(BaseAnimator<float> *a) {
        this->aPos[0] = a;
    }
    void setAnchor(glm::vec3 &&anchor) {
        this->anchor = anchor;
        refresh = true;
    }
    glm::mat4 getTransMat(float time = .0f) {
        glm::vec3 rPos = pos;
        for (int i = 0; i < 3; i++) {
            if (aPos[i] != nullptr) {
                rPos[i] = dynamic_cast<CubicBezierAnimator<float>*>(aPos[i])->at(time);
            }
        }
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, -anchor);
        trans = glm::scale(trans, scale);
        trans = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)) * trans;
        trans = glm::translate(trans, rPos);
        refresh = false;
        return trans;
    }
private:
    glm::vec3 scale{1.0f}, rotation{0}, pos{0}, anchor{0};
    BaseAnimator<float>* aPos[3]{nullptr, nullptr, nullptr};
    glm::mat4 trans;

    bool refresh;
};

#endif //STELLAR_TRANSFORM_H
