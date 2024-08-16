#include "shader/compute.h"

ComputeShader::ComputeShader(void) {
  created = false;
  attached = false;
}

bool ComputeShader::loadFile(const char* path) {
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

  return this->loadString(shader_code.c_str());
}

bool ComputeShader::loadFile(const std::string &path) {
  return this->loadFile(path.c_str());
}

bool ComputeShader::loadString(const char* code) {
  int success;
  char info_log[512];
  shader = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(shader, 1, &code, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    return false;
  }

  return true;
}

bool ComputeShader::loadString(const std::string &code) {
  return this->loadString(code.c_str());
}

bool ComputeShader::isAttached() const {
  return attached;
}

void ComputeShader::attach() {
  attached = true;
  glAttachShader(program, shader);
}

void ComputeShader::detach() {
  glDetachShader(program, shader);
  attached = false;
}

void ComputeShader::deleteShader() const {
  glDeleteShader(shader);
}

void ComputeShader::dispatch(unsigned int groups_x, unsigned int groups_y, unsigned int groups_z) const {
  glDispatchCompute(groups_x, groups_y, groups_z);
}

void ComputeShader::setMemoryBarrier(GLbitfield barrier) {
  glMemoryBarrier(barrier);
}
