#include "DQEngine.h"
#include "Player.h"
#include "Background.h"
#include "Coin.h"
#include <iostream>

int main(int argc, char** argv) {
    GameApp app(1920, 1080, "My Game");

    auto anim = new Animation2DComponent();

	auto player = new Player(); 

    //auto tilemap = new TiltedMapComponent("resource/Map/map_test.tmx");
    auto tilemap = new TiltedMapComponent("resource/Map/map1.tmx");
    auto cam = new CameraComponent(784, 441);
    auto bg = new Background();

    auto rect = new RectangleComponent(Vector(700, 200), Vector(100, 100));
    auto rect1 = new RectangleComponent(Vector(700, 100), Vector(100, 100));

    app.Add(cam);
    app.Add(rect);
    app.Add(rect1);

    cam->Follow(player);
	cam->SetBackdrop(bg);
    app.Add(tilemap);
	app.Add(new Coin(32.00, 304.00));
	app.Add(new Coin(32.00 * 2, 304.00));
	app.Add(new Coin(32.00 * 3, 304.00));
	app.Add(new Coin(32.00 * 4, 304.00));
	app.Add(new Coin(32.00 * 5, 304.00));
	app.Add(new Coin(32.00 * 6, 304.00));
    app.Add(player);

    app.Run();
    return 0;
}
