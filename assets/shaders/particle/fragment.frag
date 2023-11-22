#version 430 core

in vec3 pos;
in vec3 vel;

out vec4 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 HSV(in float h, in float s, in float value) {
   vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
   vec3 p = abs(fract(vec3(h) - K.xyz) * 6.0 - K.www);
   return value * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), s);
}

void main() {
   if (length(vec2(gl_PointCoord - vec2(0.5))) > 0.5) discard;

   FragColor = vec4(HSV(clamp(0.33 + length(vel)/150.0, 0.33, 1.0), 1.0, 1.0), 1.0);
}