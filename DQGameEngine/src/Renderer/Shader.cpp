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
    /*std::string vertexSrc = ReadFile(vertexPath);
    std::string fragmentSrc = ReadFile(fragmentPath);*/

    const char* vertexSrc = R"( 	
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec2 a_TexCoord;
        layout(location = 2) in float a_TexIndex;

        uniform mat4 u_ViewProjection; 

        out vec2 v_TexCoord;
        out float v_TexIndex;

        void main()
        {
            v_TexCoord = a_TexCoord;
            v_TexIndex = a_TexIndex;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
        }
    )";

    const char* fragmentSrc = R"( 	

        #version 330 core

        layout(location = 0) out vec4 color;

        in vec2 v_TexCoord;
        in float v_TexIndex;

        uniform sampler2D u_Textures[32];

        void main()
        {
            int index = int(v_TexIndex);

            //color = vec4(1,0,0,1); 
            color = texture(u_Textures[index], v_TexCoord);
        }

    )";

    m_ID = CompileShader(vertexSrc, fragmentSrc);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
    glUniform1i(glGetUniformLocation(m_ID, "u_Textures"), 0);
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
