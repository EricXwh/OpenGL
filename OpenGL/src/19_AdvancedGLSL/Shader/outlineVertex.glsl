#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;       // 世界坐标
out vec3 Normal;        // 法线方向

layout(std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // 转换法线
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
