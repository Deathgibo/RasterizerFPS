#pragma once
#include "Matrix4x4.h"
#include "Vec4d.h"
#include <SDL.h>
#include <iostream>
#include <chrono>
#include <time.h>
#include <algorithm>
using namespace std::chrono;
using namespace std;

struct line
{
	Vec4d p1;
	Vec4d p2;
};
class Triangle3D
{
public:
	Triangle3D()
	{
		one.x = 0; one.y = 0; one.z = 0; one.w = 1;
		two.x = 0; two.y = 0; two.z = 0; two.w = 1;
		three.x = 0; three.y = 0; three.z = 0; three.w = 1;
	}
	Triangle3D(float onex, float oney, float onez, float onew, float twox, float twoy, float twoz, float twow, float threex, float threey, float threez, float threew)
	{
		one.x = onex; one.y = oney; one.z = onez; one.w = 1;
		two.x = twox; two.y = twoy; two.z = twoz; two.w = 1;
		three.x = threex; three.y = threey; three.z = threez; three.w = 1;
		OrderPoints();
	}
	Triangle3D(Vec4d vone, Vec4d vtwo, Vec4d vthree)
	{
		one = vone;
		two = vtwo;
		three = vthree;
		r = 0;
		g = 0;
		b = 0;
	}
	void Cameratransform()
	{
		Matrix4x4 cameramatrix;
		cameramatrix.CreateCameraMatrix();

		one = cameramatrix.VectorMultiply(one);
		two = cameramatrix.VectorMultiply(two);
		three = cameramatrix.VectorMultiply(three);
	}
	void Projection()
	{

		Matrix4x4 cameramatrix;
		cameramatrix.CreateCameraMatrix();

		Matrix4x4 projectionmatrix;
		projectionmatrix.CreateProjectionMatrix();

		Matrix4x4 viewportmatrix;
		viewportmatrix.CreateViewPortMatrix();

		one = cameramatrix.VectorMultiply(one);
		two = cameramatrix.VectorMultiply(two);
		three = cameramatrix.VectorMultiply(three);
		/*Triangle3D tmp(one, two, three);
		if (tmp.calculatenormal().z < 0)
		{
			one.z = -100; two.z = -100; three.z = -100;
			return;
		}*/
		one = viewportmatrix.VectorMultiply(projectionmatrix.VectorMultiply(one));
		two = viewportmatrix.VectorMultiply(projectionmatrix.VectorMultiply(two));
		three = viewportmatrix.VectorMultiply(projectionmatrix.VectorMultiply(three));

		one.x = one.x / one.w; one.y = one.y / one.w; one.z = one.z / one.w;
		two.x = two.x / two.w; two.y = two.y / two.w; two.z = two.z / two.w;
		three.x = three.x / three.w; three.y = three.y / three.w; three.z = three.z / three.w;

	}
	void Move(float x, float y, float z)
	{
		one.Move(x, y, z, 0);
		two.Move(x, y, z, 0);
		three.Move(x, y, z, 0);
	}
	void Scale(double scalex, double scaley, double scalez)
	{
		Matrix4x4 scalematrix;
		scalematrix.CreateScaleMatrix(scalex, scaley, scalez);

		one = scalematrix.VectorMultiply(one);
		two = scalematrix.VectorMultiply(two);
		three = scalematrix.VectorMultiply(three);
	}
	void RotateX(double degrees, Vec4d center)
	{
		Matrix4x4 translatematrix;
		translatematrix.CreateTransformMatrix(-center.x, -center.y, -center.z);
		Matrix4x4 translatebackmatrix;
		translatebackmatrix.CreateTransformMatrix(center.x, center.y, center.z);
		Matrix4x4 rotatematrix;
		rotatematrix.CreateXRotationMatrix(degrees);

		//Matrix4x4 M;
		//M = translatebackmatrix * translatematrix;
		
		//one = M.VectorMultiply(one);
		//two = M.VectorMultiply(two);
		//three = M.VectorMultiply(three);
		//return;

		one = translatematrix.VectorMultiply(one);
		two = translatematrix.VectorMultiply(two);
		three = translatematrix.VectorMultiply(three);

		one = rotatematrix.VectorMultiply(one);
		two = rotatematrix.VectorMultiply(two);
		three = rotatematrix.VectorMultiply(three);


		one = translatebackmatrix.VectorMultiply(one);
		two = translatebackmatrix.VectorMultiply(two);
		three = translatebackmatrix.VectorMultiply(three);

	}
	void Rotatey(double degrees, Vec4d center)
	{
		Matrix4x4 translatematrix;
		translatematrix.CreateTransformMatrix(-center.x, -center.y, -center.z);
		Matrix4x4 translatebackmatrix;
		translatebackmatrix.CreateTransformMatrix(center.x, center.y, center.z);
		Matrix4x4 rotatematrix;
		rotatematrix.CreateYRotationMatrix(degrees);

		one = translatematrix.VectorMultiply(one);
		two = translatematrix.VectorMultiply(two);
		three = translatematrix.VectorMultiply(three);


		one = rotatematrix.VectorMultiply(one);
		two = rotatematrix.VectorMultiply(two);
		three = rotatematrix.VectorMultiply(three);


		one = translatebackmatrix.VectorMultiply(one);
		two = translatebackmatrix.VectorMultiply(two);
		three = translatebackmatrix.VectorMultiply(three);

	}
	void Rotatez(double degrees, Vec4d center)
	{
		Matrix4x4 translatematrix;
		translatematrix.CreateTransformMatrix(-center.x, -center.y, -center.z);
		Matrix4x4 translatebackmatrix;
		translatebackmatrix.CreateTransformMatrix(center.x, center.y, center.z);
		Matrix4x4 rotatematrix;
		rotatematrix.CreateZRotationMatrix(degrees);

		one = translatematrix.VectorMultiply(one);
		two = translatematrix.VectorMultiply(two);
		three = translatematrix.VectorMultiply(three);

		one = rotatematrix.VectorMultiply(one);
		two = rotatematrix.VectorMultiply(two);
		three = rotatematrix.VectorMultiply(three);

		one = translatebackmatrix.VectorMultiply(one);
		two = translatebackmatrix.VectorMultiply(two);
		three = translatebackmatrix.VectorMultiply(three);
	}
	void Transform(double xamount, double yamount)
	{
		
	}
	void ReflectYequalsB(int b)
	{
		
	}
	void ReflectYequalsnegativeX()
	{
		
	}
	void ShearX(double x)
	{
		
	}
	void ShearY(double y)
	{
		
	}
	void DrawTriangle(int r, int g, int b)
	{
		extern Vec4d cameralocation4d;
		extern vector<Triangle3D> clippedtriangles;

		Vec4d lightdirection(0,0,1,1);
		lightdirection.normalize();
		Vec4d lightcolor(255, 0, 0, 1);
		Vec4d ambientcolor(255, 255, 255, 1);
		float ambientintensity = .3;
		float lightintensity = 1;
		Vec4d color(r, g, b, 1);
		
		//clippedtriangles.push_back(*this);
		clippedtriangles = cliptrianglestart(*this);

		for (int i = 0; i < clippedtriangles.size(); i++)
		{
			if (clippedtriangles[i].normal.DotProduct(clippedtriangles[i].one - cameralocation4d) >= 0)
			{
				clippedtriangles[i].one.w = 1; clippedtriangles[i].two.w = 1; clippedtriangles[i].three.w = 1;
				
				float lightpercent = max(0, clippedtriangles[i].normal.DotProduct(lightdirection));
				lightcolor = lightcolor * lightpercent * lightintensity;
				color = color + (lightcolor * (lightpercent*lightintensity)) + (ambientcolor * ambientintensity);
				color.clamp(255);
				clippedtriangles[i].Print(color.x, color.y, color.z);
			}
			else
			{
				//cout<<"deleted";
			}
		}
		clippedtriangles.clear();
	}
	void Print(int r, int g, int b)
	{
		extern SDL_Renderer* grender;
		extern bool slow;

		Triangle3D tmp(one, two, three);
		tmp.Projection();

		SDL_SetRenderDrawColor(grender, r, g, b, 0);

		tmp.TriangleFillScanLine(r, g, b);
		//tmp.TriangleFill(r, g, b);
		//SDL_RenderDrawLine(grender, tmp.one.x, tmp.one.y, tmp.two.x, tmp.two.y);
		//SDL_RenderDrawLine(grender, tmp.two.x, tmp.two.y, tmp.three.x, tmp.three.y);
		//SDL_RenderDrawLine(grender, tmp.three.x, tmp.three.y, tmp.one.x, tmp.one.y);
		if (slow)
		{
			SDL_RenderPresent(grender);
			SDL_Delay(50);
		}
		//LineBresenhams(grender, tmp.one.x, tmp.one.y, tmp.two.x, tmp.two.y);
		//LineBresenhams(grender, tmp.two.x, tmp.two.y, tmp.three.x, tmp.three.y);
		//LineBresenhams(grender, tmp.three.x, tmp.three.y, tmp.one.x, tmp.one.y);
	}
	void Clip()
	{
		/*SDL_Rect window = { 0,0,screenwidth,screenheight };
		line lineone; lineone.p1 = one; lineone.p2 = two;
		line linetwo; linetwo.p1 = two; linetwo.p2 = three;
		line linethree; linethree.p1 = three; linethree.p2 = one;
		vector<line> lines; lines.push_back(lineone); lines.push_back(linetwo); lines.push_back(linethree);
		//see if both points are outside or both inside the square,if its true it eiterh completely visible or invisible and dealing is easy
		bool insideone = PointInsideRect(window, lines[0].p1.x, lines[0].p1.y);
		bool insidetwo = PointInsideRect(window, lines[0].p2.x, lines[0].p2.y);
		if (insideone && insidetwo)
		{
			cout << "inside";
		}
		//if(!insideone && !insidetwo)
		//get a list of all the triangles aactually intersceting square, the rest easy to deal with
			//get the intersection points, and get the number of points inside the square
			//if 1 connect that with the 2 intersection points
			//if 2 connect 2 points with 1 intersection point and again 2 with the other intersection point
			//if 0 create a polygon out of it and triangulate



		//find intersection points
		//find out quickly if a line completely inside or outside the lines
		//create new triangles or tell it to not print*/
	}
	bool PointInsideRect(SDL_Rect rect, int x, int y)
	{
		if (x < rect.x + rect.w && x > rect.x && y > rect.y && y < rect.y + rect.h)
		{
			return true;
		}
		return false;
	}
	void PrintInfo()
	{
		cout << "[ " << one.x << " " << one.y << " " << one.z << " " << one.w << " ]" << endl;
		cout << "[ " << two.x << " " << two.y << " " << two.z << " " << two.w << " ]" << endl;
		cout << "[ " << three.x << " " << three.y << " " << three.z << " " << three.w << " ]" << endl << endl;
	}
	void OrderPoints()//ordering in terms of lowest x to highest x
	{

	}
	Vec4d Center()
	{
		
	}
	vector<Triangle3D> cliptrianglestart(Triangle3D tri)
	{
		extern Vec4d facenormals[6];
		extern Vec4d facepoints[6];
		//return cliptriangle(facenormals[0], facepoints[0], tri);
		
		vector<Triangle3D> thetriangles;

		//go through every face and every new triangle gets tested along with it
		vector<Triangle3D> newtriangles;
		/*face 1*/
		thetriangles = cliptriangle(facenormals[0], facepoints[0], tri);
		//thetriangles.push_back(tri);
		//return thetriangles;
		
		vector<Triangle3D> transfertri;

		for (int i = 1; i < 6; i++)
		{
			int size = thetriangles.size();
			if (size == 0)
			{
				//break;
			}
			for (int j = 0;j<size;j++)
			{
				newtriangles = cliptriangle(facenormals[i], facepoints[i], thetriangles[j]);
				for (Triangle3D newtri : newtriangles)
				{
					transfertri.push_back(newtri);
				}
			}
			thetriangles.clear();
			thetriangles = transfertri;
			transfertri.clear();
		}
		return thetriangles;
	}
	vector<Triangle3D> cliptriangle(Vec4d n, Vec4d p, Triangle3D tri, bool otherway = 0)
	{

		//need to know the normal and point of every plane clipping
		vector<Triangle3D> triangles;

		float fa, fb, fc;
		float tmpswap;
		Vec4d vecswap;
		Vec4d A, B;
		Vec4d a, b, c;
		a = tri.one; b = tri.two; c = tri.three;
		fa = planeequation(p, n, a);
		fb = planeequation(p, n, b);
		fc = planeequation(p, n, c);
		
		/*if they are all behind plane return nothing*/
		if (fa < 0 && fb < 0 && fc < 0)
		{
			return triangles;
		}
		/*if they are all in front plane return original triangle*/
		if (fa > 0 && fb > 0 && fc > 0)
		{
			triangles.push_back(tri);
			return triangles;
		}

		//return triangles;
		if (fa*fc >= 0)
		{
			swap(fc, fb);
			swap(b, c);
			swap(fb, fa);
			swap(a, b);
		}
		else if (fb*fc >= 0)
		{
			swap(fa, fc);
			swap(a, c);
			swap(fb, fa);
			swap(b, a);
		}
		A = planeintersectpoint(p, n, a,c);
		B = planeintersectpoint(p, n, b,c);
		Triangle3D t1(a, b, A);
		Triangle3D t2(b, B, A);
		Triangle3D t3(A, B, c);
		/*now decide if c is in the plane or outside if its in the plane we pass in t3 else pass in t1 and t2*/

		if (fc <= 0)
		{
			//t1.r = 255;
			//t2.r = 255;
			t1.normal = tri.normal;
			t2.normal = tri.normal;
			triangles.push_back(t1);
			triangles.push_back(t2);
		}
		else
		{
			//t3.g = 255;
			t3.normal = tri.normal;
			triangles.push_back(t3);
		}

		return triangles;
	}

	float planeequation(Vec4d p, Vec4d n, Vec4d a)
	{
		return n.DotProduct(p - a);
	}

	float planeequationself(Vec4d p)
	{
		return normal.DotProduct(p - one);
	}
	Vec4d planeintersectpoint(Vec4d p, Vec4d n, Vec4d aorb, Vec4d c)
	{
		Vec4d A;
		Vec4d negn;
		negn = n*-1;
		float D = negn.DotProduct(p);
		float t, num, den;

		num = n.DotProduct(aorb) + D;
		den = n.DotProduct((c - aorb));
		t = -1 * (num / den);

		A = aorb + ((c-aorb)*t);

		return A;
	}

	Vec4d calculatenormal(bool reverse = 0)//clockwise
	{
		if (!reverse)
		{
			Vec4d first = two - one;
			Vec4d second = three - one;
			Vec4d n = crossproduct(first, second);
			n.normalize();

			return n;
		}
		else
		{
			Vec4d first = one - two;
			Vec4d second = one - three;
			Vec4d n = crossproduct(first, second);
			n.normalize();

			return n;
		}
	}
	bool IsClockwise()
	{
		//triangle gives us a plane equation. We have sphere with 2 points and the intersection of the plane with the sphere gives us a circle, take the angle from that

		//just find plane equations, find its basis and do regular 2d trig on it to find angles
	}
	void ReverseTriangle()
	{
		Vec4d tmp = one;
		one = three;
		three = tmp;
	}

	void SetNormal()
	{

	}

	void LineBresenhams(SDL_Renderer* grender, float xa, float ya, float xb, float yb)
	{
		SDL_SetRenderDrawColor(grender, 255, 0, 0, 0);

		//int xa, ya, xb, yb;
		int dx, dy;
		int currentx, currenty;
		int signx, signy;
		int p;
		int i;

		//xa = 100; ya = 200;
		//xb = 105; yb = 500;

		dx = xb - xa;
		dy = yb - ya;

		currentx = xa;
		currenty = ya;
		i = 0;

		if (dx >= 0)
		{
			signx = 1;
		}
		else
		{
			signx = -1;
		}
		if (dy >= 0)
		{
			signy = 1;
		}
		else
		{
			signy = -1;
		}
		if (abs(dy) < abs(dx))
		{
			p = (2 * dy*signy) - (dx * signx);
			for (i; i <= dx * signx; i++)
			{
				SDL_RenderDrawPoint(grender, currentx, currenty);
				if (p < 0)
				{
					currentx = currentx + signx;
					currenty = currenty;
					p = p + 2 * dy* signy;
				}
				else
				{
					currentx = currentx + signx;
					currenty = currenty + signy;
					p = p + (2 * dy*signy) - (2 * dx*signx);
				}
			}
		}
		else
		{
			p = 2 * dx*signx - dy * signy;
			for (i; i <= dy * signy; i++)
			{
				SDL_RenderDrawPoint(grender, currentx, currenty);
				if (p < 0)
				{
					currentx = currentx;
					currenty = currenty + signy;
					p = p + 2 * dx * signx;
				}
				else
				{
					currentx = currentx + signx;
					currenty = currenty + signy;
					p = p + (2 * dx * signx) - (2 * dy * signy);
				}
			}
		}

	}
	void TriangleFillScanLine(int r, int g, int b)
	{
		//order vertices one is very top and two is second 3 is lowest
		//cout << "before" << one.x << " " << one.y << " " << two.x << " " << two.y << " " << three.x << " " << three.y << endl;
		if (one.y > two.y)
		{
			Vec4d tmp(one.x, one.y, one.z, one.w);
			one.x = two.x; one.y = two.y; one.z = two.z; one.w = two.w;
			two.x = tmp.x; two.y = tmp.y; two.z = tmp.z; two.w = tmp.w;
		}
		if (one.y > three.y) {
			Vec4d tmp(one.x, one.y, one.z, one.w);
			one.x = three.x; one.y = three.y; one.z = three.z; one.w = three.w;
			three.x = tmp.x; three.y = tmp.y; three.z = tmp.z; three.w = tmp.w;
		}
		if (two.y > three.y)
		{
			Vec4d tmp(two.x, two.y, two.z, two.w);
			two.x = three.x; two.y = three.y; two.z = three.z; two.w = three.w;
			three.x = tmp.x; three.y = tmp.y; three.z = tmp.z; three.w = tmp.w;
		}
		//cout << "after" << one.x << " " << one.y << " " << two.x << " " << two.y << " " << three.x << " " << three.y << endl;
		if (one.y == two.y)
		{
			filltopflattriangle(r, g, b);
		}
		else if (two.y == three.y)
		{
			fillbottomflat(r, g, b);
		}
		else
		{
			//split up into two triangles and run both
			float t = (two.y - one.y) / (three.y - one.y);
			float thex = one.x + t * (three.x - one.x);
			//float newx = (one.x + ((float)(two.y - one.y) / (float)(three.y - one.y)) * (three.x - one.x));
			Vec4d four(thex, two.y, 1, 1);

			Triangle3D flatbottom(one, two, four);
			Triangle3D topbottom(two, four, three);

			flatbottom.fillbottomflat(r, g, b);
			topbottom.filltopflattriangle(r, g, b);
		}
	}
	void fillbottomflat(int r, int g, int b)
	{
		extern SDL_Renderer* grender;
		SDL_SetRenderDrawColor(grender, r, g, b, 0);

		float invslope1 = (two.x - one.x) / (two.y - one.y);
		float invslope2 = (three.x - one.x) / (three.y - one.y);

		float curx1 = one.x;
		float curx2 = one.x;

		float amount = one.y - (int)one.y;
		two.y = two.y - amount;
		three.y = three.y - amount;
		
		int scanlineY = one.y;
		for (scanlineY; scanlineY < two.y; scanlineY++)
		{
			SDL_RenderDrawLine(grender, (int)curx1, scanlineY, (int)curx2, scanlineY);
			curx1 += invslope1;
			curx2 += invslope2;
		}
		curx1 -= invslope1;
		curx2 -= invslope2;

		float percent = 1 - (scanlineY - two.y);
		curx1 += invslope1 * percent;
		curx2 += invslope2 * percent;
		SDL_RenderDrawLine(grender, (int)curx1, scanlineY, (int)curx2, scanlineY);
	}
	void filltopflattriangle(int r, int g, int b)
	{
		extern SDL_Renderer* grender;
		SDL_SetRenderDrawColor(grender, r, g, b, 0);

		float invslope1 = (three.x - one.x) / (three.y - one.y);
		float invslope2 = (three.x - two.x) / (three.y - two.y);

		float curx1 = three.x;
		float curx2 = three.x;

		float amount = ceil(three.y) - three.y;
		two.y += amount;
		one.y += amount;

		int scanlineY = ceil(three.y);
		for (scanlineY; scanlineY > one.y; scanlineY--)
		{
			SDL_RenderDrawLine(grender, (int)curx1, scanlineY, (int)curx2, scanlineY);
			curx1 -= invslope1;
			curx2 -= invslope2;
		}
		curx1 += invslope1;
		curx2 += invslope2;

		float percent = 1 - (one.y - scanlineY);
		curx1 -= invslope1*percent;
		curx2 -= invslope2*percent;
		SDL_RenderDrawLine(grender, (int)curx1, scanlineY, (int)curx2, scanlineY);
	}
	void TriangleFill(int r, int g, int b)
	{
		extern int screenwidth;
		extern int screenheight;
		extern SDL_Renderer* grender;
		
		//b + c < 1 start at a then

		SDL_SetRenderDrawColor(grender, r, g, b, 0);


		int xmax = max(one.x, max(two.x, three.x));
		int xmin = min(one.x, min(two.x, three.x));
		int ymax = max(one.y, max(two.y, three.y));
		int ymin = min(one.y, min(two.y, three.y));

		float fa = f12(one.x, one.y);
		float fb = f20(two.x, two.y);
		float fc = f01(three.x, three.y);

		for (int y = ymin; y < ymax; y++)
		{
			for (int x = xmin; x < xmax; x++)
			{
				float a = f12(x, y) / fa;
				float b = f20(x, y) / fb;
				float c = f01(x, y) / fc;
				if (a >= 0 && b >= 0 && c >= 0)
				{
					if (a > 0 || fa * f12(-1, -1) > 0)
					{
						if (b > 0 || fb * f20(-1, -1) > 0)
						{
							SDL_RenderDrawPoint(grender, x, y);
						}
					}
				}
			}
		}
	}

	float f01(int x, int y)
	{
		return (one.y - two.y)*x + (two.x - one.x)*y + one.x*two.y - two.x*one.y;
	}
	float f12(int x, int y)
	{
		return (two.y - three.y)*x + (three.x - two.x)*y + two.x*three.y - three.x*two.y;
	}
	float f20(int x, int y)
	{
		return (three.y - one.y)*x + (one.x - three.x)*y + three.x*one.y - one.x*three.y;
	}
	Vec4d crossproduct(Vec4d one, Vec4d two)
	{
		Vec4d p;
		p.x = one.y * two.z - one.z * two.y;
		p.y = one.z * two.x - one.x * two.z;
		p.z = one.x * two.y - one.y * two.x;

		return p;
	}


	Vec4d one;
	Vec4d two;
	Vec4d three;
	Vec4d normal;
	int r, g, b;

private:

};
