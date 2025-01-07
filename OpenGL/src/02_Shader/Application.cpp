#include <glad/glad.h>   // 必须在GLFW之前引入
#include <GLFW/glfw3.h>
#include <iostream>
#include <Tool/shader.h>

// 回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 窗口尺寸常量
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;


int main()
{
    // 初始化GLFW库
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置OpenGL主版本为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置OpenGL次版本为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

    // 创建一个GLFW窗口
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "无法创建GLFW窗口" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 设置当前上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 设置窗口大小回调

    // 初始化GLAD，加载所有OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD初始化失败" << std::endl;
        return -1;
    }

    Shader shader1("./src/02_Shader/Shader/vertex.glsl", "./src/02_Shader/Shader/fragmentUniform.glsl");
    Shader shader2("./src/02_Shader/Shader/vertexWithColor.glsl", "./src/02_Shader/Shader/fragment.glsl");

    // 定义第一个三角形的顶点数据（仅位置）
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // 左下
        -0.0f, -0.5f, 0.0f,  // 右下
        -0.45f, 0.5f, 0.0f   // 顶部
    };

    // 定义第二个三角形的顶点数据（位置和颜色）
    float secondTriangle[] = {
        // 位置              // 颜色
         0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下，红色
         0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下，绿色
         0.45f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部，蓝色
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // 生成两个顶点数组对象
    glGenBuffers(2, VBOs);      // 生成两个顶点缓冲对象

    // 配置第一个三角形的VAO和VBO
    glBindVertexArray(VAOs[0]); // 绑定第一个VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // 绑定第一个VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW); // 复制顶点数据

    // 配置顶点属性指针（位置）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 启用顶点属性

    glBindVertexArray(0); // 解绑VAO

    // 配置第二个三角形的VAO和VBO
    glBindVertexArray(VAOs[1]); // 绑定第二个VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // 绑定第二个VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW); // 复制顶点数据

    // 配置顶点属性指针（位置）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 启用顶点属性

    // 配置顶点属性指针（颜色）
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // 启用顶点属性

    glBindVertexArray(0); // 解绑VAO

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理用户输入
        processInput(window);

        // 清除颜色缓冲区
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);
        shader1.use();

        // 动态计算绿色分量的值
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;

        glUniform4f(glGetUniformLocation(shader1.ID, "uniformColor"), 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAOs[0]); // 绑定第一个VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

        // 绘制第二个三角形（使用顶点颜色）
        shader2.use();
        float xoffset = sin(timeValue);
        float yoffset = cos(timeValue);
        glUniform2f(glGetUniformLocation(shader2.ID, "xyoffset"), xoffset, yoffset);
        glBindVertexArray(VAOs[1]);       // 绑定第二个VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

        // 交换前后缓冲区并轮询事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源并终止GLFW
    glfwTerminate();
    return 0;
}

// 回调函数：当窗口大小改变时，调整视口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 输入处理函数：按下ESC键关闭窗口
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
