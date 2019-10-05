#pragma once
#include "Vec4d.h"
#include <vector>
#include "Triangle3D.h"

class Circle {

public:
	Circle()
	{
		radius = 0;
	}
	void Move(float x, float y, float z)
	{
		center.x += x;
		center.y += y;
		center.z += z;
	}
	void CreateCircle(Vec4d thecenter, float theradius, int r, int g, int b)
	{
		center = thecenter; center.r = r; center.g = g; center.b = b;
		radius = theradius;
	}

	std::vector<Triangle3D> ConverttoSmallerTriangles(float angleskip)//creat it then cut each triangle into 2 x many times
	{
		//plane of circle is our starting plane

		vector<Triangle3D> triangles;
		float angle = 0;
		float radianangleskip = (angleskip*3.14) / 180;

		while (angle < 360)
		{
			float radianangle = (angle * 3.14) / 180;
			Vec4d one(radius*cos(radianangle) + center.x, radius*sin(radianangle) + center.y, center.z, 1);
			radianangle += radianangleskip;
			angle += angleskip;
			Vec4d two(radius*cos(radianangle) + center.x, radius*sin(radianangle) + center.y, center.z, 1);
			Vec4d three(center.x, center.y, center.z, 1);
			Triangle3D tmp(one, two, three);
			tmp.r = center.r; tmp.g = center.g; tmp.b = center.b;
			triangles.push_back(tmp);
		}
		for (int i = 0; i < triangles.size(); i++)
		{
			triangles[i].normal = triangles[i].calculatenormal(1);
		}
		return triangles;
	}


	Vec4d center;
	float radius;
private:
};