#include "scene_loader_functions.h"
#include "loaders.h"
#include "Exceptions.h"
#include <iostream>

namespace scene_loader_functions {
    Group* getGroup(const Scene &s, const string &n, const char* info = "group or model") {
        try {
            return s.group_map.at(n);
        } catch (out_of_range &e) {
            throw name_not_defined(info);
        }
    }
    Material* getMaterial(const Scene &s, const string &n) {
        try {
            return s.material_map.at(n);
        } catch (out_of_range &e) {
            throw name_not_defined("material");
        }
    }
    unsigned int getTex(const Scene &s, const string &n) {
        try {
            return s.tex_map.at(n);
        } catch (out_of_range &e) {
            throw name_not_defined("texture");
        }
    }
}

void scene_loader_functions::obj(Scene &scene, vector<string> &v) {
    Model* model = Model::create(v[1]);
    ModelNode* group = ModelNode::create(model);
    scene.groups.push_back(group);
    scene.group_map.insert({v[2], group});
    scene.group_root->appendChild(group);
}

void scene_loader_functions::tex(Scene &scene, vector<string> &v) {
    unsigned int tex = loadTextureFromFile(v[1]);
    scene.textures.push_back(tex);
    scene.tex_map.insert({v[2], tex});
}

void scene_loader_functions::tex_cube(Scene &scene, vector<string> &v) {
    unsigned int tex = loadCubeTextureFromFile(v[1]);
    scene.textures.push_back(tex);
    scene.tex_map.insert({v[2], tex});
}

void scene_loader_functions::use_tex(Scene &scene, vector<string> &v) {
    int i = atoi(v[3].c_str());
    try {
        Group* group = scene.group_map.at(v[2]);
        if (group->is_model()) {
            dynamic_cast<ModelNode*>(group)->textures[i] = scene.tex_map.at(v[1]);
        }
    } catch (out_of_range &e) {
        std::cerr << "tex or group name not defined" << std::endl;
    }
}

void scene_loader_functions::light(Scene &scene, vector<string> &v) {
    int LIGHT_SOURCE_TYPE = atoi(v[1].c_str());
    if (LIGHT_SOURCE_TYPE == 0) {
        PointLight *light = new PointLight(
                atof(v[2].c_str()),
                atof(v[3].c_str()),
                atof(v[4].c_str()),
                atof(v[5].c_str()),
                atof(v[6].c_str()),
                atof(v[7].c_str()),
                atof(v[8].c_str())
        );
        scene.lights.push_back(light);
        scene.light_map.insert({v[9], light});
    }
}

void scene_loader_functions::light3(Scene &scene, vector<string> &v) {
    int LIGHT_SOURCE_TYPE = atoi(v[1].c_str());
    if (LIGHT_SOURCE_TYPE == 3) {
        Ambient *light = new Ambient(
                atof(v[2].c_str()),
                atof(v[3].c_str()),
                atof(v[4].c_str()),
                atof(v[5].c_str())
        );
        scene.lights.push_back(light);
        scene.light_map.insert({v[6], light});
    }
}

void scene_loader_functions::shadow(Scene &scene, vector<string> &v) {
    scene.addShadow(scene.light_map.at(v[1]));
}

void scene_loader_functions::g(Scene &scene, vector<string> &v) {
    GroupNode* group = new GroupNode();
    scene.groups.push_back(group);
    scene.group_map.insert({v[1], group});
    scene.group_root->appendChild(group);
}

void scene_loader_functions::attach(Scene &scene, vector<string> &v) {
    Group *child = getGroup(scene, v[1]),
            *parent = getGroup(scene, v[2]);
    if (parent->is_model()) {
        throw command_error("parent is a model");
    }
    dynamic_cast<GroupNode*>(parent)->appendChild(child);
}

void scene_loader_functions::pos(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.setPos({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::rotate(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.setRotation({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::scale(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.setScale({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::scalef(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.setScale(atof(v[2].c_str()));
}

void scene_loader_functions::anchor(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.setAnchor({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::mtl(Scene &scene, vector<string> &v) {
    Material *mtl = new Material();
    scene.materials.push_back(mtl);
    scene.material_map.insert({v[1], mtl});
}

void scene_loader_functions::diffuse(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->tex_color[0] = {Material::TEXTURE, tex};
}

void scene_loader_functions::diffuse_color(Scene &scene, vector<string> &v) {
    Material *mtl;
    try {
        mtl = scene.material_map.at(v[1]);
    } catch (out_of_range &e) {
        throw command_error("material name not defined");
    }
    glm::vec3 color = {
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    };
    mtl->tex_color[0] = {.t = Material::COLOR, .color = color};
}

void scene_loader_functions::use_mtl(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[2]);
    Group *model = getGroup(scene, v[1], "model");
    if (!model->is_model()) throw command_error("material can only be applied on models");
    dynamic_cast<ModelNode*>(model)->mtl = mtl;
}

void scene_loader_functions::spec(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->tex_color[1] = {Material::TEXTURE, tex};
}

void scene_loader_functions::ambient(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->tex_color[2] = {Material::TEXTURE, tex};
}

void scene_loader_functions::normal_tex(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->tex_color[2] = {Material::TEXTURE, tex};
}

void scene_loader_functions::animation(Scene &scene, vector<string> &v) {

}

void scene_loader_functions::kf(Scene &, vector<string> &) {}

void scene_loader_functions::kf_full(Scene &, vector<string> &) {}

void scene_loader_functions::skybox(Scene &scene, vector<string> &v) {
    unsigned int tex = getTex(scene, v[1]);
    scene.skybox = tex;
}