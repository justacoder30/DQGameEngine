#include "pch.h"
#include "Vector.h"

Vector Vector::Zero()
{
    return Vector(0, 0);
}

Vector Vector::All(const float& size)
{
    return Vector(size, size);
}

Vector& Vector::operator+=(Vector const& v2)
{
    x += v2.x;
    y += v2.y;
    return *this;
}

bool Vector::operator!=(Vector const& v2)
{
	return (x != v2.x || y != v2.y);
}

Vector Vector::operator+(Vector const& v2)
{
    return Vector(x + v2.x, y + v2.y);
}

Vector Vector::operator-(Vector const& v2)
{
	return Vector(x - v2.x, y - v2.y);
}

bool Vector::operator==(Vector const& v2)
{
	return (x == v2.x && y == v2.y);
}

Vector Vector::operator+(const float& num)
{
    float X = x + num;
    float Y = y + num;
    return Vector(X, Y);
}

Vector Vector::operator*(const float& num)
{
    return Vector(x * num, y * num);
}

Vector Vector::operator/(const float& num)
{
    return Vector(x / num, y / num);
}

void Vector::printf()
{
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

