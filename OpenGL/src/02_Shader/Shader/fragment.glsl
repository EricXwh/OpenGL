#version 330 core
out vec4 FragColor;

in vec3 vertexColor;

uniform vec2 xyoffset;

// ��������RGB��ɫ��תһ���Ƕȣ�����xyoffset��
vec3 rotateColors(vec3 color, float angle)
{
    // ��ת�Ƕ���[0, 2��]֮��
    float cosA = cos(angle);
    float sinA = sin(angle);
    
    // ����һ����ת������תRGB��ɫ�ռ��еĺ��̷���
    mat3 rotationMatrix = mat3(
        cosA, -sinA, 0.0,
        sinA, cosA,  0.0,
        0.0,  0.0,   1.0
    );
    
    // Ӧ����ת����
    return rotationMatrix * color;
}

void main()
{
    // ����xyoffset������ת�Ƕ�
    float angle = atan(xyoffset.y, xyoffset.x);
    
    // ��ת������ɫ
    vec3 rotatedColor = rotateColors(vertexColor, angle);
    
    FragColor = vec4(rotatedColor, 1.0f);
}
