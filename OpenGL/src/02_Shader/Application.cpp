#include <glad/glad.h>   // ������GLFW֮ǰ����
#include <GLFW/glfw3.h>
#include <iostream>
#include <Tool/shader.h>

// �ص���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���ڳߴ糣��
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;


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

    Shader shader1("./src/02_Shader/Shader/vertex.glsl", "./src/02_Shader/Shader/fragmentUniform.glsl");
    Shader shader2("./src/02_Shader/Shader/vertexWithColor.glsl", "./src/02_Shader/Shader/fragment.glsl");

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
        shader1.use();

        // ��̬������ɫ������ֵ
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;

        glUniform4f(glGetUniformLocation(shader1.ID, "uniformColor"), 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAOs[0]); // �󶨵�һ��VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // ����������

        // ���Ƶڶ��������Σ�ʹ�ö�����ɫ��
        shader2.use();
        float xoffset = sin(timeValue);
        float yoffset = cos(timeValue);
        glUniform2f(glGetUniformLocation(shader2.ID, "xyoffset"), xoffset, yoffset);
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
