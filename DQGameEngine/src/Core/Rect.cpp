#include "pch.h"
#include "Rect.h"

void Rect::setRect()
{
	top = y;
	bottom = y + h;
	left = x;
	right = x + w;
}

Rect::Rect()
{
}

Rect::Rect(const Vector& pos, const Vector& size)
{
	this->pos = pos;
	this->size = size;

	x = pos.x;
	y = pos.y;
	w = size.x;
	h = size.y;

	setRect();
}

Rect::Rect(const float& _x, const float& _y, const float& _w, const float& _h) : x(_x), y(_y), w(_w), h(_h)
{
	this->pos = Vector(x, y);
	this->size = Vector(w, h);
	setRect();
}

bool Rect::CheckCollide(const Rect& other)
{
	return (top < other.bottom and
		bottom > other.top and
		left < other.right and
		right > other.left);
}

Vector Rect::GetOverlap(const Rect& other)
{
	float dx = (x + w / 2) - (other.x + other.w / 2);
	float dy = (y + h / 2) - (other.y + other.h / 2);

	float px = (w + other.w) / 2 - abs(dx);
	float py = (h + other.h) / 2 - abs(dy);

	return Vector(px, py);
}
