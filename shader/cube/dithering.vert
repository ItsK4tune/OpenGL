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
    fragCoord = gl_Position.xy / gl_Position.w; // Normalized device coordinates [-1, 1]
    fragCoord = (fragCoord + 1.0) * 0.5 * uResolution; // Convert to screen space (viewport size)
}
