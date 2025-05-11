#version 460 core
in vec3 ourColor;
in vec2 fragCoord;

out vec4 FragColor;

// Bayer matrix 4x4
float bayerDither(vec2 coord) {
    int x = int(mod(coord.x, 4.0));
    int y = int(mod(coord.y, 4.0));
    
    int bayer4x4[4][4] = int[4][4](
        int[4]( 0,  8,  2, 10),
        int[4](12,  4, 14,  6),
        int[4]( 3, 11,  1,  9),
        int[4](15,  7, 13,  5)
    );

    int threshold = bayer4x4[y][x];
    return float(threshold) / 16.0; // Normalize to [0.0, 1.0]
}

void main()
{
    // Edge detection logic
    float dx = fwidth(fragCoord.x);
    float dy = fwidth(fragCoord.y);
    float edgeWidth = 1.5;

    float edge = smoothstep(0.0, edgeWidth, min(dx, dy));
    
    // Dither threshold
    float dither = bayerDither(fragCoord);

    // Điều chỉnh ngưỡng để điều khiển độ "lỗ"
    float cutoff = 0.8; // ← càng thấp thì càng nhiều đen (ví dụ thử 0.6, 0.5…)

    // Dither mask
    float ditherMask = step(dither, cutoff);

    // Final edge with dithering
    float finalEdge = step(0.5, edge) * ditherMask;

    // Apply to output
    FragColor = mix(vec4(0.0, 0.0, 0.0, 1.0), vec4(ourColor, 1.0), 1.0 - finalEdge);
}
