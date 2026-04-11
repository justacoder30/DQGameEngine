#include "pch.h"
#include "Components/TiltedMapComponent.h"
#include "Components/CameraComponent.h"
#include "Core/TextureManager.h"
#include "Renderer/Renderer2D.h"
#include <filesystem>

TiltedMapComponent::TiltedMapComponent(const std::string& path)
{
    Load(path);
    BuildTiles();
}

void TiltedMapComponent::Load(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "Map file NOT FOUND: " << path << std::endl;
        return;
    } else {
        std::cout << "Loading map: " << path << std::endl;
	}

    if (!m_Map.load(path))
    {
        std::cout << "Failed to load map: " << path << std::endl;
        return;
    }

    const auto& tileSets = m_Map.getTilesets();

    m_Textures.reserve(tileSets.size());

    for (auto& ts : tileSets)
    {
        m_Textures.push_back(TextureManager::Load(ts.getImagePath()));
    }

    m_TileSize = m_Map.getTileSize().x;

    auto mapSize = m_Map.getTileCount();

    m_Width = mapSize.x * m_TileSize;
    m_Height = mapSize.y * m_TileSize;
}

void TiltedMapComponent::BuildTiles()
{
    const auto& layers = m_Map.getLayers();
    const auto& tileSets = m_Map.getTilesets();

    for (auto& layer : layers)
    {
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        auto& tiles = tileLayer.getTiles();

        auto mapSize = m_Map.getTileCount();

        for (uint32_t y = 0; y < mapSize.y; y++)
        {
            for (uint32_t x = 0; x < mapSize.x; x++)
            {
                uint32_t index = y * mapSize.x + x;

                if (index >= tiles.size())
                    continue;

                auto tile = tiles[index];

                if (tile.ID == 0)
                    continue;

                for (size_t i = 0; i < tileSets.size(); i++)
                {
                    auto& ts = tileSets[i];

                    if (tile.ID < ts.getFirstGID() ||  tile.ID >= ts.getFirstGID() + ts.getTileCount())
                        continue;

                    int localID = tile.ID - ts.getFirstGID();

                    Texture* tex = m_Textures[i];

                    int tileCountX = tex->GetWidth() / m_TileSize;

					float _x, _y, _w, _h;

                    _x = (localID % tileCountX) * m_TileSize;
                    _y = (localID / tileCountX) * m_TileSize;
                    _w = m_TileSize;
                    _h = m_TileSize;
                    Rect src = Rect(_x, _y, _w, _h);

                    _x = x * m_TileSize;
                    _y = y * m_TileSize;
                    _w = m_TileSize;
                    _h = m_TileSize;
                    Rect dst = Rect(_x, _y, _w, _h);

                    Tile t;

                    t.texture = tex;
                    t.src = src;
                    t.dst = dst;

					GetAngleAndFlip(tile.flipFlags, t.angle, t.flip);

                    m_Tiles.push_back(t);

                    break;
                }
            }
        }
    }
}

void TiltedMapComponent::GetAngleAndFlip(uint8_t flags, float& angle, Flip& flip)
{
    bool h = flags & tmx::TileLayer::FlipFlag::Horizontal;
    bool v = flags & tmx::TileLayer::FlipFlag::Vertical;
    bool d = flags & tmx::TileLayer::FlipFlag::Diagonal;

    if (d) {
        if (h && !v) angle = 90.f;
        else if (!h && v) angle = 270.f;
        else if (h && v) { angle = 270.f; flip = Vertical; }
        else { angle = 90.f; flip = Vertical; }
    }
    else {
        if (h && v) angle = 180.f;
        else if (h) { angle = 0.f; flip = Horizontal; }
        else if (v) { angle = 0.f; flip = Vertical; }
    }
}

void TiltedMapComponent::OnDraw()
{
    auto cam = Renderer2D::GetCamera();
    if (!cam) return;

    Rect camRect = cam->GetViewBounds();

    for (auto& tile : m_Tiles) {

        if (!camRect.CheckCollide(tile.dst))
            continue;

        Renderer2D::Submit({
            CommandType::Sprite,
            tile.texture,
            tile.src,
            tile.dst,
            tile.flip,
            tile.angle,
			Vector(0.5, 0.5),
			//Vector(0, 0),
            layer
        });
    }
}

std::vector<Rect> TiltedMapComponent::GetObjectGroup(const std::string& name)
{
    std::vector<Rect> rects;
    const auto& mapLayers = m_Map.getLayers();
    for (const auto& layer : mapLayers) {
        if (layer->getName() == name) {
            for (const auto& obj : layer->getLayerAs<tmx::ObjectGroup>().getObjects()) {
                rects.emplace_back(obj.getPosition().x, obj.getPosition().y,
                    obj.getAABB().width, obj.getAABB().height);
            }
        }
    }
    return rects;
}
