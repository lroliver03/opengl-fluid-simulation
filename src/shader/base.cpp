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
