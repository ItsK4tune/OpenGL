#version 460 core

out vec4 FragColor;

uniform float iTime;
uniform vec2 iResolution;

// Hàm tạo bảng màu sử dụng cosine palette
vec3 palette(float t) {
    vec3 a = vec3(0.5);
    vec3 b = vec3(0.5);
    vec3 c = vec3(1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    // Lấy tọa độ pixel hiện tại
    vec2 fragCoord = gl_FragCoord.xy;

    // Tính toán uv0 từ gl_FragCoord và iResolution
    vec2 uv0 = (fragCoord * 2.0 - iResolution) / iResolution.y;
    vec2 uv1 = uv0;

    vec3 finalColor = vec3(0.0);

    // Hiệu ứng fractal nhiều lớp
    for (float i = 0.0; i < 4.0; i++) {
        uv1 = fract(uv1 * 1.5) - 0.5;
        float d = length(uv1) * exp(-length(uv0));
        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);
        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);
        finalColor += col * d;
    }

    FragColor = vec4(finalColor, 1.0);
}
