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

void Shader::createProgram() {
    program = glCreateProgram();
    created = true;
}

bool Shader::isCreated() const {
    return created;
}

bool Shader::linkProgram() {
    int success;
    char info_log[512];

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        return false;
    }

    return true;
}

void Shader::deleteProgram() const {
    glDeleteProgram(program);
}

void Shader::use() const {
    glUseProgram(program);
}

/* SET FLOAT FUNCTIONS */

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET BOOL FUNCTIONS */

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setBVec2(const std::string &name, bool x, bool y) const {
    glUniform2i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y);
}

void Shader::setBVec3(const std::string &name, bool x, bool y, bool z) const {
    glUniform3i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y, (int)z);
}

void Shader::setBVec4(const std::string &name, bool x, bool y, bool z, bool w) const {
    glUniform4i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y, (int)z, (int)w);
}

/* SET INT FUNCTIONS */

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setIVec2(const std::string &name, int x, int y) const {
    glUniform2i(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setIVec3(const std::string &name, int x, int y, int z) const {
    glUniform3i(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setIVec4(const std::string &name, int x, int y, int z, int w) const {
    glUniform4i(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET UINT FUNCTIONS */

void Shader::setUInt(const std::string &name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setUVec2(const std::string &name, unsigned int x, unsigned int y) const {
    glUniform2ui(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setUVec3(const std::string &name, unsigned int x, unsigned int y, unsigned int z) const {
    glUniform3ui(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setUVec4(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const {
    glUniform4ui(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET DOUBLE FUNCTIONS */

void Shader::setDouble(const std::string &name, double value) const {
    glUniform1d(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setDVec2(const std::string &name, double x, double y) const {
    glUniform2d(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setDVec3(const std::string &name, double x, double y, double z) const {
    glUniform3d(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setDVec4(const std::string &name, double x, double y, double z, double w) const {
    glUniform4d(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}