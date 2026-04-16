#pragma once
#include "DQEngine.h"
#include <iostream>

class Map : public TiltedMapComponent
{
public:
	Map(const std::string& path) : TiltedMapComponent(path) {}
	void OnLoad() override
	{
		std::vector<Rect> groundRects = GetObjectGroup("Collision");
		for (const auto& rect : groundRects) {
			auto r = new RectangleComponent(Vector(rect.x, rect.y), Vector(rect.w, rect.h));
			r->layer = Layer::Ground;
			r->mask = ToMask(Layer::Player);
			Add(r);
		}
	}
};

