#pragma once

#include "Renderer/Texture.h"
#include "Core/Rect.h"
#include "Core/Vector.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "Component.h"

#include <vector>
#include <string>

class TiltedMapComponent : public Component
{
private:

    struct Tile
    {
        Texture* texture;
        Rect src;
        Rect dst;

        bool flip = false;
        float rotation = 0.0f;
    };

private:

    void Load(const std::string& path);
    void BuildTiles();

    inline static tmx::Map m_Map;
    std::vector<Texture> m_Textures;
    std::vector<Tile> m_Tiles;
    int m_TileSize = 0;
    float m_Width = 0;
    float m_Height = 0;

public:

    TiltedMapComponent() = default;
    TiltedMapComponent(const std::string& path);

    void OnDraw() override;

    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
};