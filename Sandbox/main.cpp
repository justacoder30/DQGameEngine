#include "DQEngine.h"
#include "Player.h"
#include <iostream>

int main(int argc, char** argv) {
    GameApp app(1920, 1080, "My Game");

    auto anim = new Animation2DComponent();

	auto player = new Player(); 

    auto tilemap = new TiltedMapComponent("assets/demo.tmx");
	auto sprite = new SpriteComponent(new Texture("assets/images/tilemap/Run.png"));
    auto bg = new SpriteComponent(new Texture("resource/img/Background/background.png"));

	bg->width = 1920;   
	bg->height = 1080;
    
	sprite->x = 0;
	sprite->y = 0;

	app.Add(bg);
    app.Add(tilemap);
	app.Add(sprite);

    app.Add(player);

    app.Run();
    return 0;
}
