#version 450 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texture0;

void main() {
    vec4 tex = texture(texture0, texCoord);
    color = vec4(tex.r, tex.g, tex.b, 1.0f);
}