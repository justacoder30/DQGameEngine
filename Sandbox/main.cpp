#include "DQEngine.h"
#include "Player.h"
#include "Background.h"
#include <iostream>

int main(int argc, char** argv) {
    GameApp app(1920, 1080, "My Game");

    auto anim = new Animation2DComponent();

	auto player = new Player(); 

    auto tilemap = new TiltedMapComponent("assets/demo.tmx");
	auto sprite1 = new SpriteComponent(new Texture("assets/images/tilemap/tileset.png"));
    auto bg = new Background();

	bg->size.x = 1920;   
	bg->size.y = 1080;
    
    sprite1->position.x = 0;
    sprite1->position.y = 0;

	app.Add(bg);
    app.Add(tilemap);

    app.Add(player);

    app.Run();
    return 0;
}
