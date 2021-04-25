#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, msg);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << msg << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vxShader = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fgShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vxShader);
    glAttachShader(program, fgShader);
    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLEW*/
    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing glew!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // vertices fro the triangle which will be used as the data for the biffer
    float vertices[6] = {
        -0.5f, -0.5,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // creating the buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // binding the buffer with the current context
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // providing the data to the buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    // enabling vertex array
    glEnableVertexAttribArray(0);
    
    // specifying the attributes and layout of the vertices
    // index: the index of the generic vertex attribute to be modified
    // size: the number of components per generic vertex
    // type: the data type of each component in the array
    // normalized: whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE)
    // stride: the byte offset between consecutive generic vertex
    // pointer: offset of the first component of the first generic vertex attribute in the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string  vertexShader =
        "#version 330 core\n"
        "\n"
        // this location should match the 1st parameter of the function "glVertexAttribPointer"
        // although the 2nd parameter of the function "glVertexAttribPointer" is in vec2 
        // but openGL will automatically convert it to vec4
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string  fragmentShader =
        "#version 330 core\n"
        "\n"
        // this location should match the 1st parameter of the function "glVertexAttribPointer"
        // although the 2nd parameter of the function "glVertexAttribPointer" is in vec2 
        // but openGL will automatically convert it to vec4
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main(){\n"
        "   color = vec4(0.0, 1.0, 1.0, 1.0);\n" // rgba(); values from 0 to 1
        "}\n";

    unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // starting from the 1st set of vertices and drawing 3 set of vertices; total 6 co-ordinates
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        // delete the shader program
        glDeleteProgram(shaderProgram);
    }

    glfwTerminate();
    return 0;
}