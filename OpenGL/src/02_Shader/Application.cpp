#include <glad/glad.h>   // 必须在GLFW之前引入
#include <GLFW/glfw3.h>
#include <iostream>

// 回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 窗口尺寸常量
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// 顶点着色器源码（不带颜色）
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

// 顶点着色器源码（带颜色）
const char* vertexShaderWithColorSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = aColor;\n"
"}\0";

// 片段着色器源码（使用uniform颜色）
const char* fragmentShaderUniformColorSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 uniformColor;\n"
"void main()\n"
"{\n"
"   FragColor = uniformColor;\n"
"}\n\0";

// 片段着色器源码（使用顶点颜色）
const char* fragmentShaderVertexColorSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"
"}\n\0";

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

    // 编译顶点着色器（不带颜色）
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 编译顶点着色器（带颜色）
    unsigned int vertexShaderWithColor = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderWithColor, 1, &vertexShaderWithColorSource, NULL);
    glCompileShader(vertexShaderWithColor);

    // 编译片段着色器（使用uniform颜色）
    unsigned int fragmentShaderUniformColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderUniformColor, 1, &fragmentShaderUniformColorSource, NULL);
    glCompileShader(fragmentShaderUniformColor);

    // 编译片段着色器（使用顶点颜色）
    unsigned int fragmentShaderVertexColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderVertexColor, 1, &fragmentShaderVertexColorSource, NULL);
    glCompileShader(fragmentShaderVertexColor);

    // 创建并链接着色器程序1（使用uniform颜色）
    unsigned int shaderProgramUniform = glCreateProgram();
    glAttachShader(shaderProgramUniform, vertexShader);
    glAttachShader(shaderProgramUniform, fragmentShaderUniformColor);
    glLinkProgram(shaderProgramUniform);

    // 创建并链接着色器程序2（使用顶点颜色）
    unsigned int shaderProgramVertex = glCreateProgram();
    glAttachShader(shaderProgramVertex, vertexShaderWithColor);
    glAttachShader(shaderProgramVertex, fragmentShaderVertexColor);
    glLinkProgram(shaderProgramVertex);

    // 删除着色器对象，因为它们已经被链接到程序中了
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShaderWithColor);
    glDeleteShader(fragmentShaderUniformColor);
    glDeleteShader(fragmentShaderVertexColor);

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

        // 绘制第一个三角形（使用uniform颜色）
        glUseProgram(shaderProgramUniform); // 使用第一个着色器程序

        // 动态计算绿色分量的值
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;

        // 获取uniform变量的位置并设置其值
        int uniformColorLocation = glGetUniformLocation(shaderProgramUniform, "uniformColor");
        glUniform4f(uniformColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // 设置颜色为动态绿色

        glBindVertexArray(VAOs[0]); // 绑定第一个VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

        // 绘制第二个三角形（使用顶点颜色）
        glUseProgram(shaderProgramVertex); // 使用第二个着色器程序
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
