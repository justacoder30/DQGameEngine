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
        layout(location = 2) in int a_TexIndex; 
        layout(location = 3) in vec4 a_Color;

        uniform mat4 u_ViewProjection; 

        out vec2 v_TexCoord;
        flat out int v_TexIndex;
        out vec4 v_Color;

        void main() {
            v_TexCoord = a_TexCoord;
            v_TexIndex = a_TexIndex;
            v_Color = a_Color;

            gl_Position = u_ViewProjection * vec4(a_Position, 1.0); 
        }
    )";

    const char* fragmentSrc = R"( 	
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec2 v_TexCoord;
        flat in int v_TexIndex;
        in vec4 v_Color;

        uniform sampler2D u_Textures[32];

        void main()
        {
            vec4 result = vec4(1.0, 0.0, 1.0, 1.0); 

            switch(v_TexIndex)
            {
                case 0:  result = texture(u_Textures[0],  v_TexCoord); break;
                case 1:  result = texture(u_Textures[1],  v_TexCoord); break;
                case 2:  result = texture(u_Textures[2],  v_TexCoord); break;
                case 3:  result = texture(u_Textures[3],  v_TexCoord); break;
                case 4:  result = texture(u_Textures[4],  v_TexCoord); break;
                case 5:  result = texture(u_Textures[5],  v_TexCoord); break;
                case 6:  result = texture(u_Textures[6],  v_TexCoord); break;
                case 7:  result = texture(u_Textures[7],  v_TexCoord); break;
                case 8:  result = texture(u_Textures[8],  v_TexCoord); break;
                case 9:  result = texture(u_Textures[9],  v_TexCoord); break;
                case 10: result = texture(u_Textures[10], v_TexCoord); break;
                case 11: result = texture(u_Textures[11], v_TexCoord); break;
                case 12: result = texture(u_Textures[12], v_TexCoord); break;
                case 13: result = texture(u_Textures[13], v_TexCoord); break;
                case 14: result = texture(u_Textures[14], v_TexCoord); break;
                case 15: result = texture(u_Textures[15], v_TexCoord); break;
                case 16: result = texture(u_Textures[16], v_TexCoord); break;
                case 17: result = texture(u_Textures[17], v_TexCoord); break;
                case 18: result = texture(u_Textures[18], v_TexCoord); break;
                case 19: result = texture(u_Textures[19], v_TexCoord); break;
                case 20: result = texture(u_Textures[20], v_TexCoord); break;
                case 21: result = texture(u_Textures[21], v_TexCoord); break;
                case 22: result = texture(u_Textures[22], v_TexCoord); break;
                case 23: result = texture(u_Textures[23], v_TexCoord); break;
                case 24: result = texture(u_Textures[24], v_TexCoord); break;
                case 25: result = texture(u_Textures[25], v_TexCoord); break;
                case 26: result = texture(u_Textures[26], v_TexCoord); break;
                case 27: result = texture(u_Textures[27], v_TexCoord); break;
                case 28: result = texture(u_Textures[28], v_TexCoord); break;
                case 29: result = texture(u_Textures[29], v_TexCoord); break;
                case 30: result = texture(u_Textures[30], v_TexCoord); break;
                case 31: result = texture(u_Textures[31], v_TexCoord); break;
            }
            
            //result = texture(u_Textures[v_TexIndex], v_TexCoord); 
            color = result * v_Color;
        }
    )";

    m_ID = CompileShader(vertexSrc, fragmentSrc);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
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
