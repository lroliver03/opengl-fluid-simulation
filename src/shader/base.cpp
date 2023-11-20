#include "shader/base.h"

// BaseShader class member functions.

void BaseShader::createProgram() {
    program = glCreateProgram();
    created = true;
}

bool BaseShader::isCreated() const {
    return created;
}

bool BaseShader::linkProgram() {
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

void BaseShader::deleteProgram() const {
    glDeleteProgram(program);
}

void BaseShader::use() const {
    glUseProgram(program);
}

/* SET FLOAT FUNCTIONS */

void BaseShader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void BaseShader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void BaseShader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void BaseShader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET BOOL FUNCTIONS */

void BaseShader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void BaseShader::setBVec2(const std::string &name, bool x, bool y) const {
    glUniform2i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y);
}

void BaseShader::setBVec3(const std::string &name, bool x, bool y, bool z) const {
    glUniform3i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y, (int)z);
}

void BaseShader::setBVec4(const std::string &name, bool x, bool y, bool z, bool w) const {
    glUniform4i(glGetUniformLocation(program, name.c_str()), (int)x, (int)y, (int)z, (int)w);
}

/* SET INT FUNCTIONS */

void BaseShader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void BaseShader::setIVec2(const std::string &name, int x, int y) const {
    glUniform2i(glGetUniformLocation(program, name.c_str()), x, y);
}

void BaseShader::setIVec3(const std::string &name, int x, int y, int z) const {
    glUniform3i(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void BaseShader::setIVec4(const std::string &name, int x, int y, int z, int w) const {
    glUniform4i(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET UINT FUNCTIONS */

void BaseShader::setUInt(const std::string &name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
}

void BaseShader::setUVec2(const std::string &name, unsigned int x, unsigned int y) const {
    glUniform2ui(glGetUniformLocation(program, name.c_str()), x, y);
}

void BaseShader::setUVec3(const std::string &name, unsigned int x, unsigned int y, unsigned int z) const {
    glUniform3ui(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void BaseShader::setUVec4(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const {
    glUniform4ui(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

/* SET DOUBLE FUNCTIONS */

void BaseShader::setDouble(const std::string &name, double value) const {
    glUniform1d(glGetUniformLocation(program, name.c_str()), value);
}

void BaseShader::setDVec2(const std::string &name, double x, double y) const {
    glUniform2d(glGetUniformLocation(program, name.c_str()), x, y);
}

void BaseShader::setDVec3(const std::string &name, double x, double y, double z) const {
    glUniform3d(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void BaseShader::setDVec4(const std::string &name, double x, double y, double z, double w) const {
    glUniform4d(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}
