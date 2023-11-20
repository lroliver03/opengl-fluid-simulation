#include "shader/shader.h"

Shader::Shader(void) {
    created = false;
    attached[0] = false;
    attached[1] = false;
    attached[2] = false;
}

bool Shader::loadFile(ShaderType shader, const char* path) {
    std::string shader_code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(path);

        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    } catch (std::ifstream::failure err) {
        std::cout << "ERROR::SHADER::FILE_NOT_READ" << std::endl;
        return false;
    }

    return this->loadString(shader, shader_code.c_str());
}

bool Shader::loadFile(ShaderType shader, const std::string &path) {
    return this->loadFile(shader, path.c_str());
}

bool Shader::loadString(ShaderType shader, const char* code) {
    unsigned int shader_type = 
        (shader == VERTEX_SHADER)?GL_VERTEX_SHADER:
        (shader == FRAGMENT_SHADER)?GL_FRAGMENT_SHADER:
        (shader == GEOMETRY_SHADER)?GL_GEOMETRY_SHADER:
        0;
    
    if (!shader_type) {
        std::cout << "ERROR::SHADER::UNKNOWN_TYPE" << std::endl;
        return false;
    }

    int success;
    char info_log[512];
    shaders[shader] = glCreateShader(shader_type);
    glShaderSource(shaders[shader], 1, &code, NULL);
    glCompileShader(shaders[shader]);

    glGetShaderiv(shaders[shader], GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaders[shader], 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
        return false;
    }

    return true;
}

bool Shader::loadString(ShaderType shader, const std::string &code) {
    return this->loadString(shader, code.c_str());
}

bool Shader::isAttached(ShaderType shader) const {
    return attached[shader];
}

void Shader::attach(ShaderType shader) {
    attached[(int)shader] = true;
    glAttachShader(program, shaders[shader]);
}

void Shader::detach(ShaderType shader) {
    glDetachShader(program, shaders[shader]);
    attached[shader] = false;
}

void Shader::deleteShader(ShaderType shader) const {
    glDeleteShader(shaders[shader]);
}
