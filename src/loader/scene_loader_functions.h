//
// Created by baidu on 2019/2/2.
//

#include <map>
#include "Scene.h"
#include <vector>
#include <string>
#include <exception>

#ifndef STELLAR_SCENE_LOADER_FUNCTIONS_H
#define STELLAR_SCENE_LOADER_FUNCTIONS_H

using namespace std;

namespace scene_loader_functions {
    typedef void(*fun)(Scene&, vector<string>&);


    void obj(Scene&, vector<string>&);
    void tex(Scene&, vector<string>&);
    void tex_cube(Scene&, vector<string>&);
    void use_tex(Scene&, vector<string>&);
    void light(Scene&, vector<string>&);
    void light3(Scene&, vector<string>&);
    void shadow(Scene&, vector<string>&);
    void g(Scene&, vector<string>&);
    void attach(Scene&, vector<string>&);
    void pos(Scene&, vector<string>&);
    void rotate(Scene&, vector<string>&);
    void scale(Scene&, vector<string>&);
    void scalef(Scene&, vector<string>&);
    void anchor(Scene&, vector<string>&);
    void mtl(Scene&, vector<string>&);
    void diffuse(Scene&, vector<string>&);
    void diffuse_color(Scene&, vector<string>&);
    void use_mtl(Scene&, vector<string>&);
    void spec(Scene&, vector<string>&);
    void ambient(Scene&, vector<string>&);
    void normal_tex(Scene&, vector<string>&);
    void animation(Scene&, vector<string>&);
    void kf(Scene&, vector<string>&);
    void kf_full(Scene&, vector<string>&);
    void skybox(Scene&, vector<string>&);


    const map<pair<string, int>, fun> functions = {
            {{"obj", 3}, obj},
            {{"tex", 3}, tex},
            {{"tex_cube", 3}, tex_cube},
            {{"use_tex", 4}, use_tex},
            {{"light", 10}, light},
            {{"light", 7}, light3},
            {{"shadow", 2}, shadow},
            {{"g", 2}, g},
            {{"attach", 3}, attach},
            {{"pos", 5}, pos},
            {{"rotate", 5}, rotate},
            {{"scale", 5}, scale},
            {{"scale", 3}, scalef},
            {{"anchor", 5}, anchor},
            {{"mtl", 2}, mtl},
            {{"diffuse", 3}, diffuse},
            {{"diffuse", 5}, diffuse_color},
            {{"use_mtl", 3}, use_mtl},
            {{"spec", 3}, spec},
            {{"ambient", 3}, ambient},
            {{"normal_tex", 3}, normal_tex},
            {{"animation", 2}, animation},
            {{"kf", 4}, kf},
            {{"kf_full", 8}, kf_full},
            {{"skybox", 2}, skybox}
    };
}

#endif //STELLAR_SCENE_LOADER_FUNCTIONS_H
