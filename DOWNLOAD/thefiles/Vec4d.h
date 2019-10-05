#pragma once
#include <iostream>
using namespace std;

class Vec4d
{
public:
	Vec4d()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}
	Vec4d(float thex, float they, float thez, float thew, float theu = 0, float thev = 0)
	{
		x = thex;
		y = they;
		z = thez;
		w = thew;
		u = theu;
		v = thev;
	}
	Vec4d(const Vec4d &p2) 
	{
		x = p2.x;
		y = p2.y;
		z = p2.z;
		w = p2.w;
		r = p2.r;
		g = p2.g;
		b = p2.b;
	}
	void PrintInfo()
	{
		cout << "x: " << x << " y: " << y << " z: " << z << " w: " << w << endl;
	}
	void Move(float thex, float they, float thez, float thew)
	{
		x += thex;
		y += they;
		z += thez;
		w += thew;
	}
	Vec4d operator*(float other)
	{
		Vec4d tmp(x, y, z, w);
		tmp.x = tmp.x * other;
		tmp.y = tmp.y * other;
		tmp.z = tmp.z * other;

		return tmp;
	}
	Vec4d operator*(Vec4d other)
	{
		Vec4d tmp(x, y, z, w);
		tmp.x = tmp.x * other.x;
		tmp.y = tmp.y * other.y;
		tmp.z = tmp.z * other.z;

		return tmp;
	}
	Vec4d operator/(float other)
	{
		Vec4d tmp(x, y, z, w);
		tmp.x = tmp.x / other;
		tmp.y = tmp.y / other;
		tmp.z = tmp.z / other;

		return tmp;
	}
	Vec4d operator-(const Vec4d & other)
	{
		Vec4d tmp(0,0,0,1);
		tmp.x = x - other.x;
		tmp.y = y - other.y;
		tmp.z = z - other.z;

		return tmp;
	}
	Vec4d operator+(const Vec4d & other)
	{
		Vec4d tmp(0, 0, 0, 0);
		tmp.x = x + other.x;
		tmp.y = y + other.y;
		tmp.z = z + other.z;

		return tmp;
	}
	Vec4d CrossProduct(Vec4d other)
	{
		Vec4d tmp;
		tmp.x = (y * other.z) - (z * other.y);
		tmp.y = (x * other.z) - (z * other.x);
		tmp.z = (x * other.y) - (y * other.x);
		tmp.w = 1;

		return tmp;
	}
	void clamp(int value)
	{
		if (x > value)
		{
			x = value;
		}
		if (y > value)
		{
			y = value;
		}
		if (z > value)
		{
			z = value;
		}
	}
	float DotProduct(Vec4d other)
	{
		float sum;
		sum = (x * other.x) + (y* other.y) + (z * other.z);
		return sum;
	}
	Vec4d normalize()
	{
		float mag = Magnitude();
		Vec4d tmp(x, y, z, w);
		tmp.x = x / mag;
		tmp.y = y / mag;
		tmp.z = z / mag;
		
		return tmp;
	}
	float Magnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2)+ pow(z,2));
	}
	Vec4d Center(Vec4d a, Vec4d b, Vec4d c)
	{
		   
	}


	float x;
	float y;
	float z;
	float w;
	float u;
	float v;
	float r;
	float g;
	float b;

private:


};