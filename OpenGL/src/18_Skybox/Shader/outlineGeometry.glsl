#version 330 core
layout (triangles) in;               // ����������
layout (triangle_strip, max_vertices = 6) out; // �������������

in vec3 FragPos[];                   // �Ӷ�����ɫ���������������
in vec3 Normal[];                    // �Ӷ�����ɫ������ķ���

out vec3 FragPos_GS;                 // �����Ƭ����ɫ������������
out vec3 Normal_GS;                  // �����Ƭ����ɫ���ķ���

uniform mat4 projection;
uniform mat4 view;
uniform float outlineWidth;          // �����߿��

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        // ���ڷ���������չ����
        vec3 expandedPosition = FragPos[i] + Normal[i] * outlineWidth;

        // �����չ����
        gl_Position = projection * view * vec4(expandedPosition, 1.0);
        FragPos_GS = FragPos[i];
        Normal_GS = Normal[i];
        EmitVertex();
    }
    EndPrimitive();
}
