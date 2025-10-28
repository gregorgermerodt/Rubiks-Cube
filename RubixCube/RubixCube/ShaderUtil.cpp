#include "ShaderUtil.h"
#include <iostream>
#include <fstream> // file stream  

GLuint ShaderUtil::CreateShaderProgram(const char* vertexFilename, const char* fragmentFilename) // Create shader program from vertex and fragment shader files
{
    std::string vertexProgram = LoadFile(vertexFilename);           // Load vertex shader source code from file
    const char* vertexString = vertexProgram.c_str();               // Get C-string pointer from std::string
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);         // Create a vertex shader object
    glShaderSource(vertexShader, 1, &vertexString, NULL);           // Attach source code to vertex shader

    PrintOGlErrors();                                               // Print any OpenGL errors before compiling
    glCompileShader(vertexShader);                                  // Compile the vertex shader
    PrintOGlErrors();                                               // Print any OpenGL errors after compiling
    int vertCompiled;                                               // Variable to hold compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertCompiled);  // Get compile status of vertex shader
    if (vertCompiled != 1)                                          // Check if vertex shader compiled successfully
    {
        std::cout << "Vertex shader error " << std::endl;
        PrintShaderLog(vertexShader);
    }

    std::string fragmentProgram = LoadFile(fragmentFilename);
    const char* fragmentString = fragmentProgram.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentString, NULL);

    PrintOGlErrors();
    glCompileShader(fragmentShader);
    PrintOGlErrors();
    int fragCompiled;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1)
    {
        std::cout << "Fragment shader error " << std::endl;
        PrintShaderLog(fragmentShader);
    }

    GLuint shaderProgram = glCreateProgram();                       // Create a shader program object
    glAttachShader(shaderProgram, vertexShader);                    // Attach vertex shader to program
    glAttachShader(shaderProgram, fragmentShader);                  // Attach fragment shader to program
    glLinkProgram(shaderProgram);                                   // Link the shader program
    PrintOGlErrors();
    int linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);         // Get link status of shader program
    if (linked != 1)
    {
        std::cout << "Linking failed:" << std::endl;
        PrintProgramLog(shaderProgram);
    }

    // Optimization: the shaders are not relevant anymore => delete them
    glDeleteShader(vertexShader);                                  // Delete vertex shader object (no longer needed)
    glDeleteShader(fragmentShader);                                // Delete fragment shader object (no longer needed)

    return shaderProgram;                                          // Return the linked shader program ID
}

std::string ShaderUtil::LoadFile(const char* fileName)             // Load the content of a file into a std::string
{
    std::string result;                                            // String to accumulate file contents
    std::ifstream fileStream(fileName, std::ios::in);              // Open file stream for reading
    while (!fileStream.eof())
    {
        std::string line;
        std::getline(fileStream, line);                            // Read a line from file
        result.append(line + "\n");
    }
    fileStream.close();
    return result;
}

void ShaderUtil::PrintShaderLog(GLuint shader)                     // Print the info log of a shader object
{
    int memLength;                                                 // Log length
    int messageLength;                                             // Actual message length
    char* log;                                                     // Log message

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &memLength);         // Get length of shader info log
    if (memLength == 0)                                            // If no log available, return early
        return;
    log = static_cast<char*>(malloc(memLength));                   // Allocate memory for log message
    glGetShaderInfoLog(shader, memLength, &messageLength, log);    // Retrieve the info log
    std::cout << "Shader Info Log: " << log << std::endl;
    free(log);                                                     // Free allocated memory
}

void ShaderUtil::PrintProgramLog(GLuint program)                   // Print the info log of a shader program
{
    int memLength;
    int messageLength;
    char* log;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &memLength);
    if (memLength == 0)
        return;
    log = static_cast<char*>(malloc(memLength));
    glGetProgramInfoLog(program, memLength, &messageLength, log);
    std::cout << "Program Info Log: " << log << std::endl;
    free(log);
}

void ShaderUtil::PrintOGlErrors()                                  // Print all current OpenGL errors
{
    int error = glGetError();
    while (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL error: " << error << std::endl;
        error = glGetError();
    }
}
