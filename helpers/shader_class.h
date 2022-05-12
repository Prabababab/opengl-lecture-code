#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "../extern/glad/glad.h"
#include <fstream>
#include <iostream>

class shader_class{
private:
    GLuint shader;
    GLenum shader_type;
public:

    shader_class(const char* filename, GLenum shader_type);

    void compile_shader();

    void compile_status();

    void delete_shader();

    void attach_to(GLuint& shader_program);

};

#endif // SHADER_CLASS_H
