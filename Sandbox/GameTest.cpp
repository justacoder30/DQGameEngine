#include "GameTest.h"
#include "Player.h"
#include "Background.h"
#include "Coin.h"
#include "Map.h"

void GameTest::OnLoad()
{
    std::string objects_pos[] = { "PlayerPosition", "CoinPosition", "EnemyPosition", "EnemyPosition 1", "HeartPosition", "FlagPosition", "BossPosition", };

    auto anim = new Animation2DComponent();

    auto player = new Player();

    //auto tilemap = new TiltedMapComponent("resource/Map/map_test.tmx");
    auto map = new Map("resource/Map/map2.tmx");
    Add(map);
    auto cam = new CameraComponent(784, 441);
    auto bg = new Background();

    for (auto obj : objects_pos) {
        auto positions = map->GetObjectGroup(obj);

        if (positions.empty()) continue;

        if (obj == "PlayerPosition") {
            for (auto pos : positions) {
				player->SetSpawnPoint(Vector(pos.x, pos.y));
				std::cout << "Player spawn point: " << player->position.x << ", " << player->position.y << std::endl;
            }
        }
        else if (obj == "CoinPosition") {
            for (auto pos : positions) {
                Add(new Coin(pos.x, pos.y));
            }
        }
    }


    cam->Follow(player);
	cam->SetBounds(Rect(0, 0, map->GetWidth(), map->GetHeight()));

    Add(cam);
    Add(bg);
    
    Add(player);
}
