#pragma once
#include <iostream>
#include "Vec4d.h"
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
	Vec3d Rodriguesrotate(Vec4d a, float angle)
	{
		Vec4d v(x, y, z, 1);
		a = a.normalize();
		Vec4d vp = a * (a.DotProduct(v));
		Vec4d vo = v - vp;

		v = vp + (vo * cos(angle)) + (crossproduct(a, v)*sin(angle));

		Vec3d vv(v.x, v.y, v.z);
		return vv;
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
	Vec4d crossproduct(Vec4d one, Vec4d two)
	{
		Vec4d p;
		p.x = one.y * two.z - one.z * two.y;
		p.y = one.z * two.x - one.x * two.z;
		p.z = one.x * two.y - one.y * two.x;

		return p;
	}
	float Magnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	float x;
	float y;
	float z;

private:


};