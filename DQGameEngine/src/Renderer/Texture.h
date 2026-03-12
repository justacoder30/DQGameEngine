#pragma once
#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    Texture(uint32_t width, uint32_t height, void* data);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    uint32_t GetID() const { return m_ID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
	std::string GetPath() const { return m_Path; }

	bool Compare(Texture* other) const { return m_Path == other->GetPath(); }

private:
    uint32_t m_ID = 0;
    int m_Width = 0;
    int m_Height = 0;
	std::string m_Path;
};