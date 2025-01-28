#version 330 core

out vec4 FragColor;

in vec3 WorldPos;      // 来自顶点着色器的世界空间位置
in vec3 Normal;        // 来自顶点着色器的世界空间法线
in vec2 TexCoords;     // 来自顶点着色器的纹理坐标

uniform sampler2D texture_diffuse1;

// 光照相关 uniform
uniform vec3 lightDir;       // 光源方向（世界空间）
uniform vec3 lightColor;     // 光的颜色
uniform vec3 ambientColor;   // 环境光颜色

// Cel Shading 阶段数
uniform int toonSteps;       

// 光照影响系数 (0.0 ~ 1.0)，用于控制光照对最终颜色的影响
uniform float lightFactor;

// 高光相关 uniform
uniform float time;          // 动态时间，用于动画高光效果
uniform vec3 viewPos;        // 相机位置，用于高光计算

void main()
{
    // 1) 归一化法线 & 光照方向
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir); // 确保光源方向正确

    // 2) 计算漫反射强度
    float diff = max(dot(norm, lightDirNorm), 0.0);

    // 3) Cel Shading：将 diff 离散化到 toonSteps 个等级
    float stepSize = 1.0 / float(toonSteps);
    float toonValue = floor(diff / stepSize) * stepSize;

    // 4) 设置最小漫反射值，避免阴影过暗
    float minDiffuse = 0.3; // 可根据需求调整
    toonValue = max(toonValue, minDiffuse);

    // 5) 采样纹理颜色
    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;

    // 6) 计算漫反射和环境光
    vec3 diffuse = toonValue * albedo * lightColor;
    vec3 ambient = ambientColor * albedo;

    // 7) 动态高光（简单的闪烁效果）
    vec3 specular = vec3(0.0); // 将 specular 定义为 vec3
    float specThreshold = 0.9; // 高光阈值
    float shininess = 32.0;     // 高光强度

    if(dot(norm, lightDirNorm) > specThreshold)
    {
        // 计算视角方向
        vec3 viewDir = normalize(viewPos - WorldPos);
        vec3 reflectDir = reflect(lightDirNorm, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * lightColor; // 正确赋值给 vec3 specular
    }

    // 8) 计算最终光照颜色
    vec3 litColor = diffuse + ambient + specular;

    // 9) 混合光照影响与原贴图
    vec3 finalColor = mix(albedo, litColor, lightFactor);

    // 10) 输出最终颜色
    FragColor = vec4(finalColor, 1.0);
}
