#include "scene_loader_functions.h"
#include "loaders.h"
#include "Exceptions.h"
#include <iostream>


#define f(i) atof(v[i].c_str())
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

void scene_loader_functions::file(Scene &scene, vector<string> &v) {
    loadSceneFromFile(v[1], &scene);
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

void scene_loader_functions::tex_cube_convert(Scene &scene, vector<string> &v) {
    unsigned int tex = getTex(scene, v[1]);
    unsigned int tex_cube = dynamic_cast<EquirectangularShader*>(global_shaders::equirectangular_shader)->convert(tex, 1024);
    scene.tex_map.at(v[1]) = tex_cube;
    scene.textures.push_back(tex);
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
                {f(2), f(3), f(4)}, f(5),
                {f(6), f(7), f(8)}
        );
        scene.lights.push_back(light);
        scene.light_map.insert({v[9], light});
    }
}

void scene_loader_functions::light2(Scene &scene, vector<string> &v) {
    int LIGHT_SOURCE_TYPE = atoi(v[1].c_str());
    if (LIGHT_SOURCE_TYPE == 2) {
        SpotLight *light = new SpotLight(
                {f(2), f(3), f(4)}, f(5),
                {f(6), f(7), f(8)},
                {f(9), f(10), f(11)}, f(12)
        );
        scene.lights.push_back(light);
        scene.light_map.insert({v[13], light});
    }
}

void scene_loader_functions::light3(Scene &scene, vector<string> &v) {
    int LIGHT_SOURCE_TYPE = atoi(v[1].c_str());
    if (LIGHT_SOURCE_TYPE == 3) {
        Ambient *light = new Ambient(
                {f(2), f(3), f(4)}, f(5)
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
    group->transform.translate({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::rotate(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.rotate({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::scale(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.scale({
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    });
}

void scene_loader_functions::scalef(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
    group->transform.scale(atof(v[2].c_str()));
}

void scene_loader_functions::anchor(Scene &scene, vector<string> &v) {
    Group* group = scene.group_map.at(v[1]);
//    group->transform.setAnchor({
//            atof(v[2].c_str()),
//            atof(v[3].c_str()),
//            atof(v[4].c_str())
//    });
}

void scene_loader_functions::mtl(Scene &scene, vector<string> &v) {
    Material *mtl = new Material();
    scene.materials.push_back(mtl);
    scene.material_map.insert({v[1], mtl});
}

void scene_loader_functions::diffuse(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->insertTex(0, tex);
}

void scene_loader_functions::diffuse_color(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    glm::vec3 color = {
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    };
    mtl->insertColor(0, color);
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
    mtl->insertTex(1, tex);
}

void scene_loader_functions::spec_color(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    glm::vec3 color = {
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    };
    mtl->insertColor(1, color);
}

void scene_loader_functions::roughness(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->insertTex(3, tex);
}

void scene_loader_functions::roughness_color(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    glm::vec3 color = {
            atof(v[2].c_str()),
            atof(v[3].c_str()),
            atof(v[4].c_str())
    };
    mtl->insertColor(3, color);
}

void scene_loader_functions::ambient(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->insertTex(2, tex);
}

void scene_loader_functions::normal_tex(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->insertTex(2, tex);
}

void scene_loader_functions::height_map(Scene &scene, vector<string> &v) {
    Material *mtl = getMaterial(scene, v[1]);
    unsigned int tex = getTex(scene, v[2]);
    mtl->insertTex(4, tex);
}

void scene_loader_functions::animation(Scene &scene, vector<string> &v) {

}

void scene_loader_functions::kf(Scene &, vector<string> &) {}

void scene_loader_functions::kf_full(Scene &, vector<string> &) {}

void scene_loader_functions::skybox(Scene &scene, vector<string> &v) {
    unsigned int tex = getTex(scene, v[1]);
    scene.addSkybox(tex);
}
#undef f