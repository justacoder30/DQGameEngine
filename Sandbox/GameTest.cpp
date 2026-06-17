#include "GameTest.h"
#include "Player.h"
#include "Background.h"
#include "DarkMage.h"
#include "Coin.h"
#include "Map.h"
#include "Skeleton.h"

void GameTest::OnLoad()
{
    //DebugMode = true;
	std::string objects_pos[] = {"DarkMagePosition", "PlayerPosition", "CoinPosition", "EnemyPosition", "EnemyPosition 1", "HeartPosition", "FlagPosition", "BossPosition", };

    auto anim = new Animation2DComponent();
    auto player = new Player();
    auto bg = new Background();
    //auto map = new Map("resource/Map/map_test.tmx");fix
    auto map = new Map("resource/Map/map2.tmx");
    Add(map);
    Add(player);
    auto cam = new CameraComponent(784, 441);
    cam->SetOrigin(Vector(0.5f, 0.7f));
    
	bg->size = Vector(784, 441);

    for (auto obj : objects_pos) {
        auto positions = map->GetObjectGroup(obj);

		std::cout << "Object: " << obj << ", Positions Count: " << positions.size() << std::endl;

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
        else if (obj == "DarkMagePosition") {
            for (auto pos : positions) {
                Add(new DarkMage(Vector(pos.x, pos.y)));
            }
        }
    }

	auto healthbar = new Healthbar(Vector(10, 10), Vector(200, 15));  
	healthbar->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	healthbar->layer = RenderLayer::UI;
	bg->Add(healthbar);
	player->healthbar = healthbar;
    cam->Follow(player);
	cam->SetBounds(Rect(0, 0, map->GetWidth(), map->GetHeight()));

    Add(cam);
    Add(bg);
    
    SpatialGrid* grid = new SpatialGrid();
    grid->Init(256.f);
    s_Boardphase.SetBoard(grid);

    GameApp::OnLoad();
}

void GameTest::OnUpdate(float dt)
{
    m_TimeAccumulator += dt;
    m_FrameCount++;

    if (m_TimeAccumulator >= 1.0f)
    {
        float m_AverageFPS = (float)m_FrameCount / m_TimeAccumulator;
        m_TimeAccumulator -= 1.0f;
        m_FrameCount = 0;
		std::cout << "Average FPS: " << m_AverageFPS << std::endl;
        //std::cout << "Average FPS: " << 1 / dt << std::endl;
    }

    GameApp::OnUpdate(dt);
}
