#version 330 core

out vec4 FragColor;

in vec3 WorldPos;      // ���Զ�����ɫ��������ռ�λ��
in vec3 Normal;        // ���Զ�����ɫ��������ռ䷨��
in vec2 TexCoords;     // ���Զ�����ɫ������������

uniform sampler2D texture_diffuse1;

// ������� uniform
uniform vec3 lightDir;       // ��Դ��������ռ䣩
uniform vec3 lightColor;     // �����ɫ
uniform vec3 ambientColor;   // ��������ɫ

// Cel Shading �׶���
uniform int toonSteps;       

// ����Ӱ��ϵ�� (0.0 ~ 1.0)�����ڿ��ƹ��ն�������ɫ��Ӱ��
uniform float lightFactor;

// �߹���� uniform
uniform float time;          // ��̬ʱ�䣬���ڶ����߹�Ч��
uniform vec3 viewPos;        // ���λ�ã����ڸ߹����

void main()
{
    // 1) ��һ������ & ���շ���
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir); // ȷ����Դ������ȷ

    // 2) ����������ǿ��
    float diff = max(dot(norm, lightDirNorm), 0.0);

    // 3) Cel Shading���� diff ��ɢ���� toonSteps ���ȼ�
    float stepSize = 1.0 / float(toonSteps);
    float toonValue = floor(diff / stepSize) * stepSize;

    // 4) ������С������ֵ��������Ӱ����
    float minDiffuse = 0.3; // �ɸ����������
    toonValue = max(toonValue, minDiffuse);

    // 5) ����������ɫ
    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;

    // 6) ����������ͻ�����
    vec3 diffuse = toonValue * albedo * lightColor;
    vec3 ambient = ambientColor * albedo;

    // 7) ��̬�߹⣨�򵥵���˸Ч����
    vec3 specular = vec3(0.0); // �� specular ����Ϊ vec3
    float specThreshold = 0.9; // �߹���ֵ
    float shininess = 32.0;     // �߹�ǿ��

    if(dot(norm, lightDirNorm) > specThreshold)
    {
        // �����ӽǷ���
        vec3 viewDir = normalize(viewPos - WorldPos);
        vec3 reflectDir = reflect(lightDirNorm, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = spec * lightColor; // ��ȷ��ֵ�� vec3 specular
    }

    // 8) �������չ�����ɫ
    vec3 litColor = diffuse + ambient + specular;

    // 9) ��Ϲ���Ӱ����ԭ��ͼ
    vec3 finalColor = mix(albedo, litColor, lightFactor);

    // 10) ���������ɫ
    FragColor = vec4(finalColor, 1.0);
}
