#version 330 core

layout (location = 0) in vec3 aPos;       // ��������
layout (location = 1) in vec3 aNormal;    // ����
layout (location = 2) in vec2 aTexCoords; // ��������

out vec3 WorldPos;        // ����Ƭ����ɫ��������ռ�λ��
out vec3 Normal;          // ����Ƭ����ɫ��������ռ䷨��
out vec2 TexCoords;       // ����Ƭ����ɫ����UV����

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // ��������ռ�λ��
    vec4 worldPos = model * vec4(aPos, 1.0);
    WorldPos = worldPos.xyz;

    // �任����������ռ�
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // ���� UV
    TexCoords = aTexCoords;

    // ���ն�������
    gl_Position = projection * view * worldPos;
}
