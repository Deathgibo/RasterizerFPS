#pragma once
#include <iostream>
using namespace std;

class Vec3d
{
public:
	Vec3d()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vec3d(float thex, float they, float thez)
	{
		x = thex;
		y = they;
		z = thez;
	}
	Vec3d operator-(const Vec3d & other)
	{
		Vec3d tmp;
		tmp.x = x - other.x;
		tmp.y = y - other.y;
		tmp.z = z - other.z;

		return tmp;
	}
	Vec3d operator+(const Vec3d & other)
	{
		Vec3d tmp(0, 0, 0);
		tmp.x = x + other.x;
		tmp.y = y + other.y;
		tmp.z = z + other.z;

		return tmp;
	}
	Vec3d operator*(const float & other)
	{
		Vec3d tmp(x,y,z);
		tmp.x *= other;
		tmp.y *= other;
		tmp.z *= other;

		return tmp;
	}
	Vec3d operator/(const float & other)
	{
		Vec3d tmp(x, y, z);
		tmp.x /= other;
		tmp.y /= other;
		tmp.z /= other;

		return tmp;
	}
	float dotproduct(Vec3d other)
	{
		return (x*other.x) + (y*other.y) + (z*other.z);
	}

	float x;
	float y;
	float z;

private:


};