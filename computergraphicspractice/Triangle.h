#pragma once
#include "Matrix3x3.h"
#include "Vec3d.h"
#include <SDL.h>
#include <iostream>
using namespace std;

class Triangle
{
public:
	Triangle()
	{
		points.oneone = 0;
		points.onetwo = 0;
		points.onethree = 0;
		points.twoone = 0;
		points.twotwo = 0;
		points.twothree = 0;
		points.threeone = 0;
		points.threetwo = 0;
		points.threethree = 1;
	}
	Triangle(float onex, float oney, float onez, float twox, float twoy, float twoz, float threex, float threey, float threez)
	{
		points.oneone = onex;
		points.onetwo = oney;
		points.onethree = onez;
		points.twoone = twox;
		points.twotwo = twoy;
		points.twothree = twoz;
		points.threeone = threex;
		points.threetwo = threey;
		points.threethree = threez;
		OrderPoints();
	}
	void Scale(double scalex, double scaley)
	{
		Vec3d thecenter = Center();
		Matrix3x3 scalematrix;
		scalematrix.CreateScaleMatrix(scalex, scaley);
		Matrix3x3 transformMatrix;
		transformMatrix.CreateTransformMatrix(-thecenter.x, -thecenter.y);
		Matrix3x3 transformMatrixback;
		transformMatrixback.CreateTransformMatrix(thecenter.x, thecenter.y);

		Matrix3x3 thematrix = transformMatrix * scalematrix;
		thematrix = thematrix * transformMatrixback;
		points = points * thematrix;

	}
	void Rotate(double degrees)
	{
		Vec3d thecenter = Center();
		Matrix3x3 rotationmatrix;
		rotationmatrix.CreateRotationMatrix(degrees);
		Matrix3x3 transformMatrix;
		transformMatrix.CreateTransformMatrix(-thecenter.x, -thecenter.y);
		Matrix3x3 transformMatrixback;
		transformMatrixback.CreateTransformMatrix(thecenter.x, thecenter.y);

		Matrix3x3 thematrix = transformMatrix * rotationmatrix;
		thematrix = thematrix * transformMatrixback;
		points = points * thematrix;
	}
	void Transform(double xamount, double yamount)
	{
		Matrix3x3 transformMatrix;
		transformMatrix.CreateTransformMatrix(xamount, yamount);

		points = points * transformMatrix;
	}
	void ReflectYequalsB(int b)
	{
		Matrix3x3 reflectmatrix;
		reflectmatrix.CreateYequalsBMatrix(b);

		points = points * reflectmatrix;
	}
	void ReflectYequalsnegativeX()
	{
		Matrix3x3 reflectmatrix;
		reflectmatrix.CreateYequalsnegativeXMatrix();

		points = points * reflectmatrix;
	}
	void ShearX(double x)
	{
		Vec3d thecenter = Center();
		Matrix3x3 shearmatrix;
		shearmatrix.CreateShearX(x);
		Matrix3x3 transformMatrix;
		transformMatrix.CreateTransformMatrix(-points.oneone, -points.onetwo);
		Matrix3x3 transformMatrixback;
		transformMatrixback.CreateTransformMatrix(points.oneone, points.onetwo);

		Matrix3x3 thematrix = transformMatrix * shearmatrix;
		thematrix = thematrix * transformMatrixback;
		points = points * thematrix;
	}
	void ShearY(double y)
	{
		Vec3d thecenter = Center();
		Matrix3x3 shearmatrix;
		shearmatrix.CreateShearY(y);
		Matrix3x3 transformMatrix;
		transformMatrix.CreateTransformMatrix(-points.oneone, -points.onetwo);
		Matrix3x3 transformMatrixback;
		transformMatrixback.CreateTransformMatrix(points.oneone, points.onetwo);

		Matrix3x3 thematrix = transformMatrix * shearmatrix;
		thematrix = thematrix * transformMatrixback;
		points = points * thematrix;
	}
	void Print(int r, int g, int b)
	{
		extern SDL_Renderer* grender;

		SDL_SetRenderDrawColor(grender, r, g, b, 0);

		

		SDL_RenderDrawLine(grender, points.oneone, points.onetwo, points.twoone, points.twotwo);
		SDL_RenderDrawLine(grender, points.twoone, points.twotwo, points.threeone, points.threetwo);
		SDL_RenderDrawLine(grender, points.threeone, points.threetwo, points.oneone, points.onetwo);
	}
	void PrintInfo()
	{
		cout << "[ " << points.oneone << " " << points.onetwo << " " << points.onethree << " ]" << endl;
		cout << "[ " << points.twoone << " " << points.twotwo << " " << points.twothree << " ]" << endl;
		cout << "[ " << points.threeone << " " << points.threetwo << " " << points.threethree << " ]" << endl << endl;
	}
	void OrderPoints()//ordering in terms of lowest x to highest x
	{

	}
	Vec3d Center()
	{
		Vec3d tmp;
		float a = sqrt(pow(points.twoone - points.threeone, 2) + pow(points.twotwo - points.threetwo, 2));
		float b = sqrt(pow(points.oneone - points.threeone, 2) + pow(points.onetwo - points.threetwo, 2));
		float c = sqrt(pow(points.oneone - points.twoone, 2) + pow(points.onetwo - points.twotwo, 2));
		tmp.x = (a*points.oneone + b * points.twoone + c * points.threeone) / (a + b + c);
		tmp.y = (a*points.onetwo + b * points.twotwo + c * points.threetwo) / (a + b + c);

		return tmp;
	}
	Matrix3x3 points;

private:

};