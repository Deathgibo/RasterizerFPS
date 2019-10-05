#pragma once
#include "Vec4d.h"
#include "Triangle3D.h"
#include <iostream>
#include <vector>
//**need a function to triangulate the triangle into more than just 12 triangles the bigger it is, so it will still be one box for physics check
//but there will be more triangles to help with the sorting
class Box
{
public:
	Box()
	{

	}
	Box(Vec4d theone, Vec4d thetwo, Vec4d thethree, Vec4d thefour, Vec4d thefive, Vec4d thesix, Vec4d theseven, Vec4d theeight)
	{
		one = theone; two = thetwo; three = thethree; four = thefour; five = thefive; six = thesix; seven = theseven; eight = theeight;
	}
	void Move(float x, float y, float z)
	{
		one.x += x; one.y += y; one.z += z; 
		two.x += x; two.y += y; two.z += z;
		three.x += x; three.y += y; three.z += z;
		four.x += x; four.y += y; four.z += z;
		five.x += x; five.y += y; five.z += z;
		six.x += x; six.y += y; six.z += z;
		seven.x += x; seven.y += y; seven.z += z;
		eight.x += x; eight.y += y; eight.z += z;
	}
	void CreateBox(Vec4d center, float length, int r, int g, int b)
	{
		one.x = center.x - length; one.y = center.y - length; one.z = center.z - length;       	one.w = 1;    one.r = r; one.g = g; one.b = b;
		two.x = center.x + length; two.y = center.y - length; two.z = center.z - length;	   	two.w = 1;    two.r = r; two.g = g; two.b = b;
		three.x = center.x - length; three.y = center.y + length; three.z = center.z - length;  three.w = 1;  three.r = r; three.g = g; three.b = b;
		four.x = center.x + length; four.y = center.y + length; four.z = center.z - length;	     four.w = 1;  four.r = r; four.g = g; four.b = b;
		five.x = center.x - length; five.y = center.y - length; five.z = center.z + length;	   	five.w = 1;   five.r = r; five.g = g; five.b = b;
		six.x = center.x + length; six.y = center.y - length; six.z = center.z + length;	   	six.w = 1;    six.r = r; six.g = g; six.b = b;
		seven.x = center.x - length; seven.y = center.y + length; seven.z = center.z + length;  seven.w = 1;  seven.r = r; seven.g = g; seven.b = b;
		eight.x = center.x + length; eight.y = center.y + length; eight.z = center.z + length; 	eight.w = 1;  eight.r = r; eight.g = g; eight.b = b;
	}
	void CreateRectangle(Vec4d center, float width, float height, float depth, int r, int g, int b)
	{
		one.x = center.x - width; one.y = center.y - height; one.z = center.z - depth;			one.w = 1;	 one.r = r; one.g = g; one.b = b;
		two.x = center.x + width; two.y = center.y - height; two.z = center.z - depth;			two.w = 1;	 two.r = r; two.g = g; two.b = b;
		three.x = center.x - width; three.y = center.y + height; three.z = center.z - depth; three.w = 1;	 three.r = r; three.g = g; three.b = b;
		four.x = center.x + width; four.y = center.y + height; four.z = center.z - depth;     four.w = 1;	 four.r = r; four.g = g; four.b = b;
		five.x = center.x - width; five.y = center.y - height; five.z = center.z + depth;		five.w = 1;	 five.r = r; five.g = g; five.b = b;
		six.x = center.x + width; six.y = center.y - height; six.z = center.z + depth;			six.w = 1;	 six.r = r; six.g = g; six.b = b;
		seven.x = center.x - width; seven.y = center.y + height; seven.z = center.z + depth;   seven.w = 1;	 seven.r = r; seven.g = g; seven.b = b;
		eight.x = center.x + width; eight.y = center.y + height; eight.z = center.z + depth;	eight.w = 1; eight.r = r; eight.g = g; eight.b = b;
	}
	std::vector<Triangle3D> ConverttoSmallerTriangles(int cuts, bool justthetop = 0)//creat it then cut each triangle into 2 x many times
	{
		Triangle3D s1(one, two, three);
		Triangle3D s2(four, three, two);
		//right
		Triangle3D s3(four, two, eight);
		Triangle3D s4(six, eight, two);
		//left
		Triangle3D s5(three, seven, one);
		Triangle3D s6(five, one, seven);
		//back
		Triangle3D s7(five, seven, six);
		Triangle3D s8(eight, six, seven);
		//top
		Triangle3D s9(two, one, six);
		Triangle3D s10(five, six, one);
		//bottom
		Triangle3D s11(three, four, seven);
		Triangle3D s12(eight, seven, four);

		std::vector<Triangle3D> Box;

		if (justthetop)
		{
			Box.push_back(s9); Box.push_back(s10);
			std::vector<Triangle3D> boxes2;
			for (int x = 0; x < cuts; x++)
			{
				boxes2.clear();
				for (int i = 0; i < Box.size(); i++)
				{
					Vec4d midpoint;
					midpoint.x = (Box[i].three.x + Box[i].two.x) / 2;
					midpoint.y = (Box[i].three.y + Box[i].two.y) / 2;
					midpoint.z = (Box[i].three.z + Box[i].two.z) / 2;
					midpoint.w = 1;
					midpoint.r = Box[i].one.r; midpoint.g = Box[i].one.g; midpoint.b = Box[i].one.b;
					Triangle3D newone(midpoint, Box[i].one, Box[i].two);
					Triangle3D newtwo(midpoint, Box[i].three, Box[i].one);
					boxes2.push_back(newone);
					boxes2.push_back(newtwo);
				}
				Box.clear();
				Box = boxes2;
			}
			Box.push_back(s1); Box.push_back(s2); Box.push_back(s3); Box.push_back(s4);
			Box.push_back(s5); Box.push_back(s6); Box.push_back(s7); Box.push_back(s8);
			Box.push_back(s11); Box.push_back(s12);
		}
		else
		{
			Box.push_back(s1); Box.push_back(s2); Box.push_back(s3); Box.push_back(s4);
			Box.push_back(s5); Box.push_back(s6); Box.push_back(s7); Box.push_back(s8);
			Box.push_back(s9); Box.push_back(s10); Box.push_back(s11); Box.push_back(s12);

			std::vector<Triangle3D> boxes2;
			for (int x = 0; x < cuts; x++)
			{
				boxes2.clear();
				for (int i = 0; i < Box.size(); i++)
				{
					Vec4d midpoint;
					midpoint.x = (Box[i].three.x + Box[i].two.x) / 2;
					midpoint.y = (Box[i].three.y + Box[i].two.y) / 2;
					midpoint.z = (Box[i].three.z + Box[i].two.z) / 2;
					midpoint.w = 1;
					midpoint.r = Box[i].one.r; midpoint.g = Box[i].one.g; midpoint.b = Box[i].one.b;
					Triangle3D newone(midpoint, Box[i].one, Box[i].two);
					Triangle3D newtwo(midpoint, Box[i].three, Box[i].one);
					boxes2.push_back(newone);
					boxes2.push_back(newtwo);
				}
				Box.clear();
				Box = boxes2;
			}
		}
		
		for (int i = 0; i < Box.size(); i++)
		{
			Box[i].normal = Box[i].calculatenormal(1);
		}
		return Box;
	}
	std::vector<Triangle3D> Converttotriangles()
	{
		Triangle3D s1(one, two, three);
		Triangle3D s2(two, four, three);
		//right
		Triangle3D s3(two, eight, four);
		Triangle3D s4(six, eight, two);
		//left
		Triangle3D s5(one, three, seven);
		Triangle3D s6(five, one, seven);
		//back
		Triangle3D s7(five, seven, six);
		Triangle3D s8(six, seven, eight);
		//top
		Triangle3D s9(one, six, two);
		Triangle3D s10(five, six, one);
		//bottom
		Triangle3D s11(three, four, seven);
		Triangle3D s12(seven, four, eight);

		std::vector<Triangle3D> Box;
		Box.push_back(s1); Box.push_back(s2); Box.push_back(s3); Box.push_back(s4);
		Box.push_back(s5); Box.push_back(s6); Box.push_back(s7); Box.push_back(s8);
		Box.push_back(s9); Box.push_back(s10); Box.push_back(s11); Box.push_back(s12);
		for (int i = 0; i < Box.size(); i++)
		{
			Box[i].normal = Box[i].calculatenormal(1);
		}
		return Box;
	}
	bool isboxvalid()
	{
		if (one.x == five.x && two.x == six.x && three.x == seven.x && four.x == eight.x)
		{
			return true;
		}
		return false;
	}
	Vec4d one;
	Vec4d two;
	Vec4d three;
	Vec4d four;
	Vec4d five;
	Vec4d six;
	Vec4d seven;
	Vec4d eight;
private:


};

//order is starting with front... topleft,topright,bottomleft,bottomright, then the back in that order