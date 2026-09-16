#include "pch.h"
#include "TextureManager.h"

namespace dqengine {

std::unordered_map<std::string, SharedPtr<Texture>> TextureManager::s_Textures;

SharedPtr<Texture> TextureManager::Load(const std::string& path)
{
    const auto it = s_Textures.find(path);
    if (it != s_Textures.end()) return it->second;
    auto texture = path == "default" ? Shared<Texture>() : Shared<Texture>(path);
    s_Textures.emplace(path, texture);
    return texture;
}
void TextureManager::Clear() { s_Textures.clear(); }

} // namespace dqengine
