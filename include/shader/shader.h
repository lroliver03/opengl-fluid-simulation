#pragma once

#include "glad/glad.h"
#include "base.h"

class Shader : public BaseShader {
  protected:
    GLuint shaders[3];
    bool attached[3];

  public:
    enum ShaderType {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      GEOMETRY_SHADER
    };

    Shader(void);

    bool loadFile(ShaderType shader, const char* path);
    bool loadFile(ShaderType shader, const std::string &path);
    bool loadString(ShaderType shader, const char* code);
    bool loadString(ShaderType shader, const std::string &code);
    
    bool isAttached(ShaderType shader) const;
    void attach(ShaderType shader);
    void detach(ShaderType shader);
    void deleteShader(ShaderType shader) const;
};
