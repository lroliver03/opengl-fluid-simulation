#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aVel;

out vec3 pos;
out vec3 vel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   gl_Position = model * vec4(aPos, 1.0);
   pos = aPos;
   vel = aVel;
}