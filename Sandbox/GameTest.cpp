#include "GameTest.h"
#include "Player.h"
#include "Background.h"
#include "Coin.h"
#include "Map.h"
#include "Skeleton.h"

void GameTest::OnLoad()
{
	std::string objects_pos[] = { "PlayerPosition", "CoinPosition", "EnemyPosition", "EnemyPosition 1", "HeartPosition", "FlagPosition", "BossPosition", };

    auto anim = new Animation2DComponent();

    auto player = new Player();
    auto bg = new Background();
    //auto map = new Map("resource/Map/map_test.tmx");
    auto map = new Map("resource/Map/map2.tmx");
    Add(map);
    Add(player);
    auto cam = new CameraComponent(784, 441);
    
	bg->size = Vector(784, 441);

    for (auto obj : objects_pos) {
        auto positions = map->GetObjectGroup(obj);

        if (positions.empty()) continue;

        if (obj == "PlayerPosition") {
            for (auto pos : positions) {
				player->SetSpawnPoint(Vector(pos.x, pos.y));
				
            }
        }
        else if (obj == "CoinPosition") {
            for (auto pos : positions) {
                Add(new Coin(pos.x, pos.y));
            }
        }
        else if (obj == "EnemyPosition") {
            for (auto pos : positions) {
                Add(new Skeleton(Vector(pos.x, pos.y)));
            }
        }
    }

    //auto coin = new Coin(0, 0);
    //coin->layer = RenderLayer::UI;
    ////coin->position = Vector(-784/2, -441/2);
    //bg->Add(coin);


	auto healthbar = new Healthbar(Vector(10, 10), Vector(200, 15));  
	healthbar->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	healthbar->layer = RenderLayer::UI;
	bg->Add(healthbar);

	player->healthbar = healthbar;

    cam->Follow(player);
	cam->SetBounds(Rect(0, 0, map->GetWidth(), map->GetHeight()));

    Add(cam);
    Add(bg);
    //std::cout << bg->coin->position.x << ", " << bg->coin->position.y << std::endl;
    
    SpatialGrid* grid = new SpatialGrid();
    grid->Init(256.f);
    s_Boardphase.SetBoard(grid);

	//m_Running = true;

    

	
}

void GameTest::OnUpdate(float dt)
{

    //std::cout << "Coin World Position: " << coin_pos.x << ", " << coin_pos.y << ". hp_pos = " << hp_pos.x << ", " << hp_pos.y << std::endl;
    GameApp::OnUpdate(dt);
}
