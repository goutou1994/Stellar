//
// Created by baidu on 2019/1/29.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace camera_global {
    CenteredSphereCamera *camera;

    void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
        static float lastX = 0, lastY = 0;
        float sensitivity = 0.4f;
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;
        camera->pitchAngle += yoffset * sensitivity;
        camera->yawAngle += - xoffset * sensitivity;
    }
}

glm::mat4 CenteredSphereCamera::getTransMat() {
    glm::mat4 proj = glm::perspective(glm::radians(fieldAngle), aspectRatio, 0.1f, 1000.0f);
    glm::vec3 eye = getCameraPosition();
    glm::vec3 head = cos(glm::radians(pitchAngle)) > 0 ? glm::vec3(0, 1.0f, 0) : glm::vec3(0, -1.0f, 0);
    glm::mat4 view = glm::lookAt(eye, lookat, head);
    glm::mat4 trans = proj * view;
    return trans;
}

glm::mat4 CenteredSphereCamera::getTransMat_NoTranslate() {
    glm::mat4 proj = glm::perspective(glm::radians(fieldAngle), aspectRatio, 0.1f, 1000.0f);
    glm::vec3 eye = getCameraPosition();
    glm::vec3 head = cos(glm::radians(pitchAngle)) > 0 ? glm::vec3(0, 1.0f, 0) : glm::vec3(0, -1.0f, 0);
    glm::mat4 view = glm::lookAt(glm::vec3(0), lookat - eye, head);
    return proj * view;
}

glm::vec3 CenteredSphereCamera::getCameraPosition() {
    glm::vec3 eye;
    eye.x = cos(glm::radians(pitchAngle)) * sin(glm::radians(yawAngle)) * radius;
    eye.y = sin(glm::radians(pitchAngle)) * radius;
    eye.z = cos(glm::radians(pitchAngle)) * cos(glm::radians(yawAngle)) * radius;
    return this->lookat + eye;
}

void CameraControl::start(float sensitivity) {
    camera_global::camera = &this->camera;
    glfwSetCursorPosCallback(window, camera_global::cursor_callback);
}

glm::vec3 LookAtCamera::getCameraPosition() {
    return this->eye;
}

glm::mat4 LookAtCamera::getTransMat() {
    glm::mat4 proj = glm::perspective(glm::radians(fieldAngle), aspectRatio, 1.0f, 200.0f);
    glm::vec3 head = glm::vec3(0, 1.0f, 0);
    glm::mat4 view = glm::lookAt(eye, lookat, head);
    glm::mat4 trans = proj * view;
    return trans;
}