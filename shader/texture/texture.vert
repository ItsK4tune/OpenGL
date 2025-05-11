#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 uModel;

void main() {
    gl_Position = uModel * vec4(aPos, 0.0, 1.0);
    uv = aUV;
}