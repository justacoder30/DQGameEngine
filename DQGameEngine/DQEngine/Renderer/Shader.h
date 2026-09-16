#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:

    Shader(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetIntArray(const std::string& name, int* values, uint32_t count);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

private:

    std::string ReadFile(const std::string& path);

    GLuint CompileShader(
        const std::string& vertexSrc,
        const std::string& fragmentSrc
    );

private:

    GLuint m_ID;
};