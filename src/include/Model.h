//
// Created by baidu on 2019/1/22.
//

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <assimp/scene.h>

#ifndef STELLAR_MODEL_H
#define STELLAR_MODEL_H

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
};

struct Texture {
    GLuint id;
    std::string type;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<Texture>&& texture);
    void draw();
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
    void calcTangent();
};

class Model {
public:
    static Model* create(std::string path) {
        Model* ptr = new Model(path);
        return ptr;
    }
    void draw();
private:
    Model(std::string path) {
        loadModel(path);
    }

    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif //STELLAR_MODEL_H
