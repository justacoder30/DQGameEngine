#pragma once
#include "Vector.h"

class Rect
{
public:

	Rect();
	Rect(const Vector& pos, const Vector& size);
	Rect(const float& _x, const float& _y, const float& _w, const float& _h);
	bool CheckCollide(const Rect& other);
	Vector GetOverlap(const Rect& other);
	Vector GetCenter();
	Vector GetMTV(const Rect& other);
	
	Vector pos, size;
	float x, y, w, h;
	float top, bottom, left, right = 0;

private:

	void setRect();
};

