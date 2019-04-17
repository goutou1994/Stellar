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
//        refresh = true;
        this->trans = glm::mat4(1.f);
        this->linear_trans = glm::mat4(1.f);
    }
    void scale(glm::vec3 &&scale) {
        this->trans = glm::scale(this->trans, scale);
        this->linear_trans = glm::scale(this->trans, scale);
    }
    void scale(float s) {
        this->scale(glm::vec3(s));
    }
    void rotate(glm::vec3 &&rotation) {
        this->trans = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)) * trans;
        this->linear_trans = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)) * linear_trans;
    }
    void translate(glm::vec3 &&translate) {
        this->trans = glm::translate(this->trans, translate);
    }

    glm::mat4 getTransMat(float time = .0f) {
//        glm::vec3 rPos = pos;
//        for (int i = 0; i < 3; i++) {
//            if (aPos[i] != nullptr) {
//                rPos[i] = dynamic_cast<CubicBezierAnimator<float>*>(aPos[i])->at(time);
//            }
//        }
        return this->trans;
    }
    glm::mat4 getLinearTransMat(float time = .0f) {
        return this->linear_trans;
    }
private:
//    glm::vec3 scale{1.0f}, rotation{0}, pos{0}, anchor{0};
//    BaseAnimator<float>* aPos[3]{nullptr, nullptr, nullptr};
    glm::mat4 trans;
    glm::mat4 linear_trans;

//    bool refresh;
};

#endif //STELLAR_TRANSFORM_H
