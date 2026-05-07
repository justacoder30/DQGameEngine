#pragma once
#include <string>

class Texture
{
public:

    Texture(const std::string& path);
    Texture(uint32_t width, uint32_t height, void* data);
    Texture();
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    const uint32_t& GetID() { return m_ID; }
    const int& GetWidth() { return m_Width; }
    const int& GetHeight() { return m_Height; }

	bool Compare(Texture* other) const { return m_ID == other->GetID(); }

private:
    uint32_t m_ID = 0;
    int m_Width = 0;
    int m_Height = 0;
};