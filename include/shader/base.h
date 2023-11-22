#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class BaseShader {
    protected:
        bool created;

    public:
        GLuint program;
        void createProgram();
        bool isCreated() const;
        bool linkProgram();
        void deleteProgram() const;
        
        void use() const;
        
        // Removed.
};