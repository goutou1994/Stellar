#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Model.h"
#include "Shaders.h"
#include "Camera.h"
#include "loaders.h"

const int SCR_WIDTH = 500, SCR_HEIGHT = 300;

int main() {

    //----------------------init window-----------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "shadow", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //-----------------------------end----------------------------

    Scene scene = loadSceneFromFile("test_scene.txt");
    CubicBezierAnimator<float>* animator = new CubicBezierAnimator<float>();
//    scene.groups[0]->transform.setPosX(animator);
    animator->addKeyFrame(0, 0);
    animator->addKeyFrame(3, 5);
    animator->generate();

    //--------------------------prepare-------------------------------

//    LookAtCamera camera(90.0f, 1.0f, glm::vec3(0, 0, 50.f), glm::vec3(0, 0, 0));
    CenteredSphereCamera camera;
    camera.radius = 50.0f;
    camera.aspectRatio = (float)SCR_WIDTH / SCR_HEIGHT;
    CameraControl camera_control(window, camera);
    camera_control.start();
    scene.camera = &camera;

    global_shaders::init();

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    GLint num;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &num);
    std::cout << num << std::endl;

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    int res;
    std::cin >> res;

    glfwTerminate();
    return 0;
}