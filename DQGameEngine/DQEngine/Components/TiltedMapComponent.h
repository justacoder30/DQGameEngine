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

namespace dqengine {

class TiltedMapComponent : public PositionComponent
{
private:

    struct Tile
    {
        SharedPtr<Texture> texture;
        Rect src;
        Rect dst;

        Flip flip = None;
        float angle = 0.0f;
    };

private:

    void Load(const std::string& path);
    void BuildTiles();
	void GetAngleAndFlip(uint8_t flags, float& angle, Flip& flip);

    tmx::Map m_Map;
    std::vector<SharedPtr<Texture>> m_Textures;
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
    std::vector<Rect> GetObjectGroup(const std::string& name);
};
} // namespace dqengine
