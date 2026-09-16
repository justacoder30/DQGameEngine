#pragma once
#include <unordered_map>
#include <string>
#include "Renderer/Texture.h"

namespace dqengine {

class TextureManager
{
public:
    static SharedPtr<Texture> Load(const std::string& path = "default");
    static void Clear();

private:
    static std::unordered_map<std::string, SharedPtr<Texture>> s_Textures;
};
} // namespace dqengine
