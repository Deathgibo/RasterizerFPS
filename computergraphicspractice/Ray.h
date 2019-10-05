#pragma once
#include "Vec4d.h"

class Ray {

public:
	Ray()
	{
		
	}

	void setray(float px, float py, float pz, float dx, float dy, float dz)
	{
		position.x = px; position.y = py; position.z = pz; position.w = 1;
		direction.x = dx; direction.y = dy; direction.z = dz; direction.w = 1;
	}

	void setray(Vec4d rayposition, Vec4d raydirection)
	{
		position = rayposition;
		direction = raydirection;
	}

	Vec4d position;
	Vec4d direction; //normalized
private:

};