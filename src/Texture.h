//
// Created by simon on 10.04.17.
//

#pragma once

#include <cstdint>
#include <GL/glew.h>
#include <string>

enum TextureDimension {
    TWO,
    THREE
};

class Shader;

class Texture {
public:
    Texture(uint32_t width, uint32_t height, uint32_t length, TextureDimension dimension, GLint wrapMethod, GLint filter, std::string name);
    ~Texture();
    void setData(GLfloat* data, GLint internalFormat, GLenum format);
    void activate(Shader* shader);

    static GLuint textureIndex;
private:
    uint32_t _width, _height, _length;
    TextureDimension _dimension;
    GLint _wrap, _filter;
    std::string _name;
    GLuint _textureId, _textureIndex;
};


