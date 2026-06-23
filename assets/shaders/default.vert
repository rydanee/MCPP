#version 450 core

layout (location = 0) in vec3 aPos;

uniform float screenRatio;

void main() {
    gl_position = vec4(aPos, 1.0f);
}