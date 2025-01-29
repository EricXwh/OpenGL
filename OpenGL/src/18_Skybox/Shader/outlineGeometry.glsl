#version 330 core
layout (triangles) in;               // 输入三角形
layout (triangle_strip, max_vertices = 6) out; // 输出三角形条带

in vec3 FragPos[];                   // 从顶点着色器传入的世界坐标
in vec3 Normal[];                    // 从顶点着色器传入的法线

out vec3 FragPos_GS;                 // 输出给片段着色器的世界坐标
out vec3 Normal_GS;                  // 输出给片段着色器的法线

uniform mat4 projection;
uniform mat4 view;
uniform float outlineWidth;          // 轮廓线宽度

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        // 基于法线向外扩展顶点
        vec3 expandedPosition = FragPos[i] + Normal[i] * outlineWidth;

        // 输出扩展顶点
        gl_Position = projection * view * vec4(expandedPosition, 1.0);
        FragPos_GS = FragPos[i];
        Normal_GS = Normal[i];
        EmitVertex();
    }
    EndPrimitive();
}
