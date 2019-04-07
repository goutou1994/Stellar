#include "Model.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<Texture>&& textures)
    : vertices(vertices), indices(indices), textures(textures) {
    calcTangent();
    setupMesh();
}

#define u(i) v[i]->TexCoord.x
#define p(i) v[i]->Position
void Mesh::calcTangent() {
    for (auto p = indices.cbegin(); p < indices.cend(); p += 3) {
        std::vector<Vertex>::iterator v[3] = {
                vertices.begin() + *p,
                vertices.begin() + *(p + 1),
                vertices.begin() + *(p + 2)
        };
        float t = (u(0) - u(1)) / (u(2) - u(1));
        glm::vec3 tangent = -glm::normalize(p(1) + t * (p(2) - p(1)) - p(0));

        // TODO temporary
        v[0]->Tangent = tangent;
        v[1]->Tangent = tangent;
        v[2]->Tangent = tangent;
    }
}
#undef u
#undef p

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    // 设置顶点坐标指针
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    // 设置法线指针
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, Normal));
    // 设置顶点的纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, TexCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, Tangent));
    
    glBindVertexArray(0);
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}