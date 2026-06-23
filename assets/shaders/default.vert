#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texPos;

uniform float scRatio = 1;

out vec2 texCoord;

void main() {
    gl_Position = vec4((aPos.x * scRatio), aPos.y, aPos.z, 1.0f);
    texCoord = texPos;
}