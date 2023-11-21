#pragma once

#include "glad/glad.h"
#include "base.h"

class ComputeShader : public BaseShader {
    protected:
        GLuint shader;
        bool attached;
    
    public:
        ComputeShader(void);

        bool loadFile(const char* path);
        bool loadFile(const std::string &path);
        bool loadString(const char* code);
        bool loadString(const std::string &code);

        bool isAttached() const;
        void attach();
        void detach();
        void deleteShader() const;

        void dispatch(unsigned int groups_x, unsigned int groups_y, unsigned int groups_z) const;
        void setMemoryBarrier(GLbitfield barrier);
};
