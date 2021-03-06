//
// Created by oliver on 17.12.16.
//


#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/vec3.hpp>

class Shader;
class Character;

class TextRenderer {
public:
    TextRenderer(std::string path);
    ~TextRenderer();
    void drawText(std::string text, glm::vec2 pos, GLfloat scale, glm::vec3 color);

private:
    void setupShader();
    void prepareFreeType();
    void manageVertexObjects();
    std::string _pathToFont;
    std::map<GLchar, Character> _Characters;
    Shader* _shader;
    GLuint _VBO, _VAO;
};
