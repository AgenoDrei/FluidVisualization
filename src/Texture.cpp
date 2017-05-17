//
// Created by simon on 10.04.17.
//

#include <iostream>
#include "Texture.h"
#include "Shader/Shader.h"

GLuint Texture::textureIndex = 0;

Texture::Texture(uint32_t width, uint32_t height, uint32_t length, TextureDimension dimension, GLint wrapMethod, GLint filter, std::string name) :
    _width(width),
    _height(height),
    _length(length),
    _dimension(dimension),
    _wrap(wrapMethod),
    _filter(filter),
    _name(name)
{
    _textureIndex = ++Texture::textureIndex;
    std::cout << "Created Texture with index: " << _textureIndex << std::endl;
}

Texture::~Texture() {}

void Texture::setData(GLfloat *data, GLint internalFormat, GLenum format) {
    std::cout << "Texture '" << _name << "' index at data binding: " << textureIndex << std::endl;
    glGenTextures(1, &_textureId);
    glActiveTexture(GL_TEXTURE0 + _textureIndex);
    if (_dimension == TWO) {
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else if (_dimension == THREE) {
        glBindTexture(GL_TEXTURE_3D, _textureId);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S,
                        _wrap);      // GL_CLAMP_TO_BORDER instead of GL_CLAMP resolved GL_INVALID_ENUM error
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, _wrap);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, _wrap);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, _filter);
        //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, _filter);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
        glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, _width, _height, _length, 0, format, GL_FLOAT,
                     data); //Remove magic numbers
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, 100, 100, 100, 0, GL_RED, GL_UNSIGNED_BYTE, pData); //Remove magic numbers
        //glGenerateMipmap(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, 0);
    }
}

void Texture::activate(Shader* shader) {
    //std::cout << "Texture '" << _name.c_str() << "' index at activation: " << _textureIndex << std::endl;
    glActiveTexture(GL_TEXTURE0 + _textureIndex);
    if (_dimension == TWO) {
        glBindTexture(GL_TEXTURE_2D, _textureId);
    } else if(_dimension == THREE) {
        glBindTexture(GL_TEXTURE_3D, _textureId);
    }
    glUniform1i(glGetUniformLocation(shader->Program, _name.c_str()), _textureIndex);

}

void Texture::reuseTextures() {
    Texture::textureIndex = 0;
}