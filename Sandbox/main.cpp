#include "DQEngine.h"
#include "Player.h"
#include <iostream>

int main(int argc, char** argv) {
    GameApp app(1920, 1080, "My Game");

    auto anim = new Animation2DComponent();

	auto player = new Player(); 

    auto tilemap = new TiltedMapComponent("assets/demo.tmx");
	auto sprite1 = new SpriteComponent(new Texture("assets/images/tilemap/tileset.png"));
	auto sprite2 = new SpriteComponent(new Texture("assets/images/tilemap/tileset.png"));
    auto bg = new SpriteComponent(new Texture("resource/img/Background/background.png"));

	bg->width = 1920;   
	bg->height = 1080;
    
    sprite1->x = 0;
    sprite1->y = 0;

    sprite2->x = 11.5;
    sprite2->y = 0;

	app.Add(bg);
    app.Add(tilemap);
	app.Add(sprite1);
    app.Add(sprite2);


    app.Add(player);

    app.Run();
    //Test1();
    return 0;
}
