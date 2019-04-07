//
// Created by baidu on 2019/1/31.
//

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#ifndef STELLAR_CAMERA_H
#define STELLAR_CAMERA_H

class BaseCamera {
public:
    float fieldAngle, aspectRatio;
    BaseCamera(float fa = 60.0f, float ar = 1.0f) : fieldAngle(fa), aspectRatio(ar) {}
    virtual glm::mat4 getTransMat() = 0;
    virtual glm::mat4 getTransMat_NoTranslate() = 0;
    virtual glm::vec3 getCameraPosition() = 0;
};

class LookAtCamera : public BaseCamera {
public:
    glm::vec3 lookat, eye;
    LookAtCamera(float fa, float ar, glm::vec3 eye, glm::vec3 lookat)
            : BaseCamera(fa, ar), lookat(lookat), eye(eye) {}
    glm::mat4 getTransMat();
    glm::vec3 getCameraPosition();
};

class CenteredSphereCamera : public BaseCamera {
public:
    float pitchAngle, yawAngle, rollAngle, radius;
    glm::vec3 lookat;
    CenteredSphereCamera(float fa, float ar, glm::vec3 lookat, float pa, float ya, float ra, float r)
            : BaseCamera(fa, ar), lookat(lookat), pitchAngle(pa), yawAngle(ya), rollAngle(ra), radius(r) {}
    CenteredSphereCamera() : CenteredSphereCamera(60, 1, {0, 0, 0}, 0, 0, 0, 1) {}

    glm::mat4 getTransMat();
    glm::vec3 getCameraPosition();
    glm::mat4 getTransMat_NoTranslate();
};

class CameraControl {
public:
    CameraControl(GLFWwindow* window, CenteredSphereCamera& camera) : window(window), camera(camera) {}
    void start(float sensitivity = 1.0f);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);

private:
    GLFWwindow* window;
    CenteredSphereCamera& camera;
    float sensitivity;
};

#endif //STELLAR_CAMERA_H
