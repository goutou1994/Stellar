#include "Shaders.h"

namespace global_shaders {
    Shader *normal_shader;
    Shader *point_light_shadow_shader;
    Shader *debug_shader;
    Shader *cube_tex_debug_shader;
    Shader *pbr_shader;
    Shader *current_shader;
    Shader *skybox_shader;
}

void global_shaders::init() {
    current_shader = nullptr;
    normal_shader = new TestShader("shaders/one-point-light-shadowed/v.glsl",
                                   "shaders/one-point-light-shadowed/f.glsl");
    pbr_shader = new PBRShader("shaders/basic-pbr/v.glsl", "shaders/basic-pbr/f.glsl");
    point_light_shadow_shader = new PointLightShadowShader(
            "shaders/shadow/point_light_v.glsl",
            "shaders/shadow/point_light_f.glsl",
            "shaders/shadow/point_light_g.glsl"
    );
    debug_shader = new DebugShader("shaders/debug/v.glsl", "shaders/debug/f.glsl", "shaders/debug/g.glsl");
    cube_tex_debug_shader = new CubeTexDebugShader("shaders/cube-tex-debug/v.glsl", "shaders/cube-tex-debug/f.glsl",
                                                   "shaders/cube-tex-debug/g.glsl");
    skybox_shader = new SkyboxShader("shaders/skybox/v.glsl", "shaders/skybox/f.glsl");
}

