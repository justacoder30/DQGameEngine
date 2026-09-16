#pragma once
#include <unordered_map>
#include <string>
#include "Renderer/Texture.h"

class TextureManager
{
public:
    static Texture* Load(const std::string& path = "default");
    static void Clear();

private:
    static std::unordered_map<std::string, Texture*> s_Textures;
};