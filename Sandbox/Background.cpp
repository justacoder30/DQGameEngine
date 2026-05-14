#include "Background.h"

void Background::OnLoad()
{
	size = Renderer2D::GetViewportSize();

	std::vector<std::string> bg_layer = {
		"resource/img/Background/1.png",
		"resource/img/Background/2.png",
		"resource/img/Background/3.png",
		"resource/img/Background/4.png",
	};

	std::vector<SpriteComponent*> bgSprites = {
		new SpriteComponent(bg_layer[0]),
		new SpriteComponent(bg_layer[1]),
		new SpriteComponent(bg_layer[1]),
		new SpriteComponent(bg_layer[2]),
		new SpriteComponent(bg_layer[3]),
	};

	for (size_t i = 0; i < bgSprites.size(); i++)
	{
		bgSprites[i]->position = Vector(0, 0);
		bgSprites[i]->size = size;
		bgSprites[i]->layer = RenderLayer::Background;
		Add(bgSprites[i]);
		if (i == 1 || i == 2) clouds.push_back(bgSprites[i]);
	}
	clouds[1]->position.x = size.x;
	
}

void Background::OnUpdate(float dt)
{
	for (size_t i = 0; i < clouds.size(); i++)
	{
		clouds[i]->position.x -= cloud_speed * dt;
		if (clouds[i]->position.x <= -size.x) clouds[i]->position.x = size.x;
	}

	PositionComponent::OnUpdate(dt);
}
