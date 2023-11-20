#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    GLuint program;
    bool created;
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

        void createProgram();
        bool isCreated() const;
        bool linkProgram();
        void deleteProgram() const;
        
        void use() const;
        
        // Since GLFW has different functions for every type, it forced my hand.
        
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;
        
        void setBool(const std::string &name, bool value) const;
        void setBVec2(const std::string &name, bool x, bool y) const;
        void setBVec3(const std::string &name, bool x, bool y, bool z) const;
        void setBVec4(const std::string &name, bool x, bool y, bool z, bool w) const;
        
        void setInt(const std::string &name, int value) const;
        void setIVec2(const std::string &name, int x, int y) const;
        void setIVec3(const std::string &name, int x, int y, int z) const;
        void setIVec4(const std::string &name, int x, int y, int z, int w) const;
        
        void setUInt(const std::string &name, unsigned int value) const;
        void setUVec2(const std::string &name, unsigned int x, unsigned int y) const;
        void setUVec3(const std::string &name, unsigned int x, unsigned int y, unsigned int z) const;
        void setUVec4(const std::string &name, unsigned int x, unsigned int y, unsigned int z, unsigned int w) const;
        
        void setDouble(const std::string &name, double value) const;
        void setDVec2(const std::string &name, double x, double y) const;
        void setDVec3(const std::string &name, double x, double y, double z) const;
        void setDVec4(const std::string &name, double x, double y, double z, double w) const;
};

#endif