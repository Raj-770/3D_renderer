#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Manages the creation, compilation, linking, and usage of OpenGL shader programs.
 * 
 *        - This class loads GLSL shader source files, compiles vertex and fragment shaders,
 *        links them into an OpenGL program, and provides convinience functions for setting
 *        unforms.
 * 
 * @param vertexPath path to vertex shader source file
 * @param fragmentPath path to fragment shader source file
 */
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : ID_(0)
{
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Link shaders into a program
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    checkCompileErrors(ID_, "PROGRAM");

    // Delete shader objects now linked to the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/**
 * @brief Destructor releases the OpenGL shader program resource
 * 
 */
Shader::~Shader() {
    if (ID_) glDeleteProgram(ID_);
}

/**
 * @brief Move constructor
 * 
 * @param other 
 */
Shader::Shader(Shader&& other) noexcept : ID_(other.ID_) {
    other.ID_ = 0;
}

/**
 * @brief Move assignment operator
 * 
 * @param other 
 * @return Shader& 
 */
Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (ID_) glDeleteProgram(ID_);
        ID_ = other.ID_;
        other.ID_ = 0;
    }
    return *this;
}

/**
 * @brief Activates the shader program for rendering.
 * 
 */
void Shader::use() const {
    glUseProgram(ID_);
}

/**
 * @brief Sets a boolean uniform value in the shader program/
 * 
 * @param name Uniform variable name
 * @param value Boolean value to set
 */
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

/**
 * @brief Sets an integer uniform value in the shader program.
 * 
 * @param name Uniform variable name.
 * @param value Integer value to set.
 */

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

/**
 * @brief Sets a float uniform value in the shader program.
 * 
 * @param name Uniform variable name.
 * @param value Float value to set.
 */
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

/**
 * @brief Sets a vec3 (3-component float) uniform in the shader program.
 * 
 * @param name Uniform variable name.
 * @param value glm::vec3 value to set.
 */
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]);
}

/**
 * @brief Sets a mat4 (4x4 float matrix) uniform in the shader program.
 * 
 * @param name Uniform variable name.
 * @param mat glm::mat4 value to set.
 */

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

/**
 * @brief Loads the contents of a shader source file into a string. Throws on failure.
 * 
 * @param filepath Path to the shader source file.
 * @return The entire source code as a string.
 */
std::string Shader::loadShaderSource(const std::string& filepath) const {
    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open shader file: " + filepath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Checks for compilation or linking errors and logs them to std::cerr.
 * 
 * @param shader The shader/program handle.
 * @param type Either "VERTEX", "FRAGMENT", or "PROGRAM".
 */
void Shader::checkCompileErrors(GLuint shader, const std::string& type) const {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR: Shader compilation error (type " << type << "):\n"
                      << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR: Program linking error:\n" << infoLog << std::endl;
        }
    }
}
