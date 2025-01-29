#version 330 core

layout (location = 0) in vec3 aPos;       // 顶点坐标
layout (location = 1) in vec3 aNormal;    // 法线
layout (location = 2) in vec2 aTexCoords; // 纹理坐标

out vec3 WorldPos;        // 传至片段着色器：世界空间位置
out vec3 Normal;          // 传至片段着色器：世界空间法线
out vec2 TexCoords;       // 传至片段着色器：UV坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 计算世界空间位置
    vec4 worldPos = model * vec4(aPos, 1.0);
    WorldPos = worldPos.xyz;

    // 变换法线至世界空间
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // 传递 UV
    TexCoords = aTexCoords;

    // 最终顶点坐标
    gl_Position = projection * view * worldPos;
}
