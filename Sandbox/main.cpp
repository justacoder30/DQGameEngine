#include "DQEngine.h"
#include "Player.h"
#include <iostream>

int main(int argc, char** argv) {
    GameApp app(1920, 1080, "My Game");

    auto anim = new Animation2DComponent();

	auto player = new Player(); 

    auto tilemap = new TiltedMapComponent("assets/demo.tmx");
	auto sprite = new SpriteComponent(new Texture("assets/images/tilemap/Run.png"));

	sprite->x = 0;
	sprite->y = 0;

    app.Add(tilemap);
	app.Add(sprite);

    app.Add(player);

    app.Run();
    return 0;
}
