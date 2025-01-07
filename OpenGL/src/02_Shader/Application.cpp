#include <glad/glad.h>   // ������GLFW֮ǰ����
#include <GLFW/glfw3.h>
#include <iostream>

// �ص���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���ڳߴ糣��
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// ������ɫ��Դ�루������ɫ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

// ������ɫ��Դ�루����ɫ��
const char* vertexShaderWithColorSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = aColor;\n"
"}\0";

// Ƭ����ɫ��Դ�루ʹ��uniform��ɫ��
const char* fragmentShaderUniformColorSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 uniformColor;\n"
"void main()\n"
"{\n"
"   FragColor = uniformColor;\n"
"}\n\0";

// Ƭ����ɫ��Դ�루ʹ�ö�����ɫ��
const char* fragmentShaderVertexColorSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"
"}\n\0";

int main()
{
    // ��ʼ��GLFW��
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ����OpenGL���汾Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ����OpenGL�ΰ汾Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ

    // ����һ��GLFW����
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "�޷�����GLFW����" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // ���õ�ǰ������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ���ô��ڴ�С�ص�

    // ��ʼ��GLAD����������OpenGL����ָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD��ʼ��ʧ��" << std::endl;
        return -1;
    }

    // ���붥����ɫ����������ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ���붥����ɫ��������ɫ��
    unsigned int vertexShaderWithColor = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderWithColor, 1, &vertexShaderWithColorSource, NULL);
    glCompileShader(vertexShaderWithColor);

    // ����Ƭ����ɫ����ʹ��uniform��ɫ��
    unsigned int fragmentShaderUniformColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderUniformColor, 1, &fragmentShaderUniformColorSource, NULL);
    glCompileShader(fragmentShaderUniformColor);

    // ����Ƭ����ɫ����ʹ�ö�����ɫ��
    unsigned int fragmentShaderVertexColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderVertexColor, 1, &fragmentShaderVertexColorSource, NULL);
    glCompileShader(fragmentShaderVertexColor);

    // ������������ɫ������1��ʹ��uniform��ɫ��
    unsigned int shaderProgramUniform = glCreateProgram();
    glAttachShader(shaderProgramUniform, vertexShader);
    glAttachShader(shaderProgramUniform, fragmentShaderUniformColor);
    glLinkProgram(shaderProgramUniform);

    // ������������ɫ������2��ʹ�ö�����ɫ��
    unsigned int shaderProgramVertex = glCreateProgram();
    glAttachShader(shaderProgramVertex, vertexShaderWithColor);
    glAttachShader(shaderProgramVertex, fragmentShaderVertexColor);
    glLinkProgram(shaderProgramVertex);

    // ɾ����ɫ��������Ϊ�����Ѿ������ӵ���������
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShaderWithColor);
    glDeleteShader(fragmentShaderUniformColor);
    glDeleteShader(fragmentShaderVertexColor);

    // �����һ�������εĶ������ݣ���λ�ã�
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // ����
        -0.0f, -0.5f, 0.0f,  // ����
        -0.45f, 0.5f, 0.0f   // ����
    };

    // ����ڶ��������εĶ������ݣ�λ�ú���ɫ��
    float secondTriangle[] = {
        // λ��              // ��ɫ
         0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ���£���ɫ
         0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ���£���ɫ
         0.45f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ��������ɫ
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // �������������������
    glGenBuffers(2, VBOs);      // �����������㻺�����

    // ���õ�һ�������ε�VAO��VBO
    glBindVertexArray(VAOs[0]); // �󶨵�һ��VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // �󶨵�һ��VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW); // ���ƶ�������

    // ���ö�������ָ�루λ�ã�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // ���ö�������

    glBindVertexArray(0); // ���VAO

    // ���õڶ��������ε�VAO��VBO
    glBindVertexArray(VAOs[1]); // �󶨵ڶ���VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // �󶨵ڶ���VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW); // ���ƶ�������

    // ���ö�������ָ�루λ�ã�
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // ���ö�������

    // ���ö�������ָ�루��ɫ��
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // ���ö�������

    glBindVertexArray(0); // ���VAO

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // �����û�����
        processInput(window);

        // �����ɫ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ����������ɫ
        glClear(GL_COLOR_BUFFER_BIT);

        // ���Ƶ�һ�������Σ�ʹ��uniform��ɫ��
        glUseProgram(shaderProgramUniform); // ʹ�õ�һ����ɫ������

        // ��̬������ɫ������ֵ
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;

        // ��ȡuniform������λ�ò�������ֵ
        int uniformColorLocation = glGetUniformLocation(shaderProgramUniform, "uniformColor");
        glUniform4f(uniformColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // ������ɫΪ��̬��ɫ

        glBindVertexArray(VAOs[0]); // �󶨵�һ��VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����������

        // ���Ƶڶ��������Σ�ʹ�ö�����ɫ��
        glUseProgram(shaderProgramVertex); // ʹ�õڶ�����ɫ������
        glBindVertexArray(VAOs[1]);       // �󶨵ڶ���VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����������

        // ����ǰ�󻺳�������ѯ�¼�
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // �ͷ���Դ����ֹGLFW
    glfwTerminate();
    return 0;
}

// �ص������������ڴ�С�ı�ʱ�������ӿڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ���봦����������ESC���رմ���
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
