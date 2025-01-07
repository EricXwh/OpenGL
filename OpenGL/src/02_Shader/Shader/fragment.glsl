#version 330 core
out vec4 FragColor;

in vec3 vertexColor;

uniform vec2 xyoffset;

// 函数：将RGB颜色旋转一定角度（基于xyoffset）
vec3 rotateColors(vec3 color, float angle)
{
    // 旋转角度在[0, 2π]之间
    float cosA = cos(angle);
    float sinA = sin(angle);
    
    // 创建一个旋转矩阵，旋转RGB颜色空间中的红绿分量
    mat3 rotationMatrix = mat3(
        cosA, -sinA, 0.0,
        sinA, cosA,  0.0,
        0.0,  0.0,   1.0
    );
    
    // 应用旋转矩阵
    return rotationMatrix * color;
}

void main()
{
    // 根据xyoffset计算旋转角度
    float angle = atan(xyoffset.y, xyoffset.x);
    
    // 旋转顶点颜色
    vec3 rotatedColor = rotateColors(vertexColor, angle);
    
    FragColor = vec4(rotatedColor, 1.0f);
}
