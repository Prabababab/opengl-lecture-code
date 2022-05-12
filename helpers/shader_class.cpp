#include "shader_class.h"

shader_class::shader_class(const char* filename, GLenum shader_type){

    this->shader_type = shader_type;

    std::ifstream in(filename, std::ios::binary);
    std::string contents;
    if(in){
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
    }else{
        std::cout << "File Reading failed :(\n";
        throw(errno);
    }

    //Create the Shader given the shader enum
    shader = glCreateShader(shader_type);

    const char* shader_source = contents.c_str();
    glShaderSource(shader, 1, &shader_source, NULL);

}

void shader_class::compile_shader(){
    glCompileShader(shader);
}

void shader_class::compile_status(){
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::COMPILATION_FAILED" << ((shader_type == GL_VERTEX_SHADER) ? "::VERTEX::SHADER\n" : "::FRAGMENT::SHADER\n") << infoLog << std::endl;
    }
}

void shader_class::delete_shader(){
    glDeleteShader(shader);
}

void shader_class::attach_to(GLuint& shader_program){
    glAttachShader(shader_program, shader);
}
