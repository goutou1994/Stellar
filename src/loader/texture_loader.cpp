#include "loaders.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

void bindTexture(std::string path, GLenum target) {
    int width, height, nrChannels;
    unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb);
    if (textureData) {
        glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }

    stbi_image_free(textureData);
}

GLuint loadCubeTextureFromFile(std::string path) {
    static std::map<std::string, GLuint> texture_loaded;
    if (texture_loaded.find(path) != texture_loaded.end()) {
        return texture_loaded.at(path);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        char s[50];
        sprintf(s, path.c_str(), i);
        bindTexture(s, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    texture_loaded.insert({path, texture});

    return texture;
}


GLuint loadTextureFromFile(std::string path) {

    static std::map<std::string, GLuint> texture_loaded;

    if (texture_loaded.find(path) != texture_loaded.end()) {
        return texture_loaded.at(path);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char* textureData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (textureData) {
        GLenum format = GL_RGB;

        if (nrChannels == 4) {
            format = GL_RGBA;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 2) {
            format = GL_RG;
        } else if (nrChannels == 1) {
            format = GL_RED;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        stbi_image_free(textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture_loaded.insert({path, texture});
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    return texture;
}