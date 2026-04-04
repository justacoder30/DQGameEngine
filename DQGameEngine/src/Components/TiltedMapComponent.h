#pragma once

#include "Core/Rect.h"
#include "Core/Vector.h"
#include "Core/TextureManager.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "PositionComponent.h"
#include "Core/Flip.h"

#include <vector>
#include <string>

class TiltedMapComponent : public PositionComponent
{
private:

    struct Tile
    {
        Texture* texture;
        Rect src;
        Rect dst;

        Flip flip = None;
        float angle = 0.0f;
    };

private:

    void Load(const std::string& path);
    void BuildTiles();
	void GetAngleAndFlip(uint8_t flags, float& angle, Flip& flip);

    inline static tmx::Map m_Map;
    std::vector<Texture*> m_Textures;
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