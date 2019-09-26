#pragma once
#include <iostream>
using namespace std;

class Matrix3x3
{
public:
	Matrix3x3()
	{
		oneone = 0;
		onetwo = 0;
		onethree = 0;
		twoone = 0;
		twotwo = 0;
		twothree = 0;
		threeone = 0;
		threetwo = 0;
		threethree = 0;
	}
	Matrix3x3(float a, float b, float c, float d, float e, float f, float g, float h, float j)
	{
		oneone = a;
		onetwo = b;
		onethree = c;
		twoone = d;
		twotwo = e;
		twothree = f;
		threeone = g;
		threetwo = h;
		threethree = j;
	}
	Matrix3x3& operator*(const Matrix3x3 & other)
	{
		Matrix3x3 tmp;
		tmp.oneone = oneone*other.oneone + onetwo*other.twoone + onethree* other.threeone;
		tmp.onetwo = oneone * other.onetwo + onetwo * other.twotwo + onethree * other.threetwo;
		tmp.onethree = oneone * other.onethree + onetwo * other.twothree + onethree * other.threethree;
		tmp.twoone = twoone * other.oneone + twotwo * other.twoone + twothree * other.threeone;
		tmp.twotwo = twoone * other.onetwo + twotwo * other.twotwo + twothree * other.threetwo;
		tmp.twothree = twoone * other.onethree + twotwo * other.twothree + twothree * other.threethree;
		tmp.threeone = threeone * other.oneone + threetwo * other.twoone + threethree * other.threeone;
		tmp.threetwo = threeone * other.onetwo + threetwo * other.twotwo + threethree * other.threetwo;
		tmp.threethree = threeone * other.onethree + threetwo * other.twothree + threethree * other.threethree;

		return tmp;
	}
	void CreateScaleMatrix(double scalex, double scaley)
	{
		oneone = scalex; onetwo = 0; onethree = 0;
		twoone = 0; twotwo = scaley; twothree = 0;
		threeone = 0; threetwo = 0; threethree = 1;
	}
	void CreateYequalsBMatrix(int b)
	{
		oneone = 1; onetwo = 0; onethree = 0;
		twoone = 0; twotwo = -1; twothree = 0;
		threeone = 0; threetwo = 2*b; threethree = 1;
	}
	void CreateYequalsnegativeXMatrix()
	{
		oneone = 0; onetwo = 1; onethree = 0;
		twoone = 1; twotwo = 0; twothree = 0;
		threeone = 0; threetwo = 0; threethree = 1;
	}
	void CreateShearX(double x)
	{
		oneone = 1; onetwo = 0; onethree = 0;
		twoone = x; twotwo = 1; twothree = 0;
		threeone = 0; threetwo = 0; threethree = 1;
	}
	void CreateShearY(double y)
	{
		oneone = 1; onetwo = y; onethree = 0;
		twoone = 0; twotwo = 1; twothree = 0;
		threeone = 0; threetwo = 0; threethree = 1;
	}
	void CreateRotationMatrix(double degrees)
	{
		degrees = (degrees * M_PI) / 180;
		oneone = cos(degrees); onetwo = sin(degrees); onethree = 0;
		twoone = -sin(degrees); twotwo = cos(degrees); twothree = 0;
		threeone = 0; threetwo = 0; threethree = 1;
	}
	void CreateTransformMatrix(double x, double y)
	{
		threeone = x;
		threetwo = y;
		oneone = 1;
		twotwo = 1;
		threethree = 1;
	}
	void PrintInfo()
	{
		cout << "[ " << oneone << " " << onetwo << " " << onethree << " ]" << endl;
		cout << "[ " << twoone << " " << twotwo << " " << twothree << " ]" << endl;
		cout << "[ " << threeone << " " << threetwo << " " << threethree << " ]" << endl << endl;
	}
	float oneone;
	float onetwo;
	float onethree;
	float twoone;
	float twotwo;
	float twothree;
	float threeone;
	float threetwo;
	float threethree;

private:

};