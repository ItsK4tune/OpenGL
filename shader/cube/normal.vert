#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec2 fragCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec2 uResolution;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
    ourColor = aColor;
}
