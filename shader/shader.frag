#version 450

// 从顶点着色器接收颜色
layout(location = 0) in vec3 fragColor;

// 输出最终像素颜色
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}