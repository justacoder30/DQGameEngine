#include "pch.h"
#include "Shader.h"

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    if (source.empty()) std::cerr << "Shader file is empty: " << path << std::endl;
    else  std::cout << "Shader loaded: " << path << std::endl;

    return source;
}

GLuint Shader::CompileShader(
    const std::string& vertexSrc,
    const std::string& fragmentSrc)
{
    GLuint program = glCreateProgram();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vertexSrc.c_str();

    glShaderSource(vertex, 1, &vsrc, nullptr);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fragmentSrc.c_str();

    glShaderSource(fragment, 1, &fsrc, nullptr);
    glCompileShader(fragment);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

Shader::Shader(
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    std::string vertexSrc = ReadFile(vertexPath);
    std::string fragmentSrc = ReadFile(fragmentPath);

    m_ID = CompileShader(vertexSrc, fragmentSrc);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
    glUniform1i(glGetUniformLocation(m_ID, "uTexture"), 0);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
{
    glUniform1iv(
        glGetUniformLocation(m_ID, name.c_str()),
        count,
        values
    );
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    // glGetUniformLocation: Tìm vị trí của biến uniform trong shader
    // 1: Số lượng ma trận truyền vào
    // GL_FALSE: Không hoán vị (transpose) ma trận vì GLM đã khớp với định dạng của OpenGL
    // glm::value_ptr: Lấy con trỏ đến dữ liệu mảng của ma trận
    glUniformMatrix4fv(
        glGetUniformLocation(m_ID, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}
