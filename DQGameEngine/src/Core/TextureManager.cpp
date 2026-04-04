#include "pch.h"
#include "TextureManager.h"

std::unordered_map<std::string, Texture*> TextureManager::s_Textures;

Texture* TextureManager::Load(const std::string& path)
{
    if (s_Textures.find(path) != s_Textures.end())
        return s_Textures[path];  

    Texture* tex = new Texture(path);
    s_Textures[path] = tex;
    return tex;
}

void TextureManager::Clear()
{
    for (auto& pair : s_Textures)
        delete pair.second;

    s_Textures.clear();
}
