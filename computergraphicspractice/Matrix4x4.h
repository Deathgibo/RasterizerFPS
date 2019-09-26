#pragma once
#include <iostream>
#include "Vec4d.h"
#include "Vec3d.h"
using namespace std;

class Matrix4x4
{
public:
	Matrix4x4()
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
		fourone = 0;
		fourtwo = 0;
		fourthree = 0;
		fourfour = 0;
	}
	Matrix4x4(float a, float b, float c, float d, float e, float f, float g, float h, float j, float k, float l, float m, float n, float o, float p, float r)
	{
		oneone = a;
		onetwo = b;
		onethree = c;
		onefour = d;
		twoone = e;
		twotwo = f;
		twothree = g;
		twofour = h;
		threeone = j;
		threetwo = k;
		threethree = l;
		threefour = m;
		fourone = n;
		fourtwo = o;
		fourthree = p;
		fourfour = r;
	}
	Matrix4x4& operator*(const Matrix4x4 & other)
	{
		Matrix4x4 tmp;

		tmp.oneone = oneone * other.oneone + onetwo * other.twoone + onethree * other.threeone + onefour * other.fourone;
		tmp.onetwo = oneone * other.onetwo + onetwo * other.twotwo + onethree * other.threetwo + onefour * other.fourtwo;
		tmp.onethree = oneone * other.onethree + onetwo * other.twothree + onethree * other.threethree + onefour * other.threefour;
		tmp.onefour = oneone * other.onefour + onetwo * other.twofour + onethree * other.threefour + onefour * other.fourfour;

		tmp.twoone = twoone * other.oneone + twotwo * other.twoone + twothree * other.threeone + twofour * other.fourone;
		tmp.twotwo = twoone * other.onetwo + twotwo * other.twotwo + twothree * other.threetwo + twofour * other.fourtwo;
		tmp.twothree = twoone * other.onethree + twotwo * other.twothree + twothree * other.threethree + twofour * other.fourthree;
		tmp.twofour = twoone * other.onefour + twotwo * other.twofour + twothree * other.threefour + twofour * other.fourfour;

		tmp.threeone = threeone * other.oneone + threetwo * other.twoone + threethree * other.threeone + threefour * other.fourone;
		tmp.threetwo = threeone * other.onetwo + threetwo * other.twotwo + threethree * other.threetwo + threefour * other.fourtwo;
		tmp.threethree = threeone * other.onethree + threetwo * other.twothree + threethree * other.threethree + threefour * other.fourthree;
		tmp.threefour = threeone * other.onefour + threetwo * other.twofour + threethree * other.threefour + threefour * other.fourfour;
		
		tmp.fourone = fourone * other.oneone + fourtwo * other.twoone + fourthree * other.threeone + fourfour * other.fourone;
		tmp.fourtwo = fourone * other.onetwo + fourtwo * other.twotwo + fourthree * other.threetwo + fourfour * other.fourtwo;
		tmp.fourthree = fourone * other.onethree + fourtwo * other.twothree + fourthree * other.threethree + fourfour * other.fourthree;
		tmp.fourfour = fourone * other.onefour + fourtwo * other.twofour + fourthree * other.threefour + fourfour * other.fourfour;

		return tmp;
	}
	Vec4d VectorMultiply(Vec4d vec)
	{
		Vec4d tmp;

		tmp.x = vec.x * oneone + vec.y * onetwo + vec.z * onethree + vec.w * onefour;
		tmp.y = vec.x * twoone + vec.y * twotwo + vec.z * twothree + vec.w * twofour;
		tmp.z = vec.x * threeone + vec.y * threetwo + vec.z * threethree + vec.w * threefour;
		tmp.w = vec.x * fourone + vec.y * fourtwo + vec.z * fourthree + vec.w * fourfour;

		return tmp;
	}
	void CreateViewPortMatrix()
	{
		extern int screenwidth;
		extern int screenheight;

		oneone = screenwidth/2; onetwo = 0;					onethree = 0;     onefour = (screenwidth)/2;
		twoone = 0;				twotwo = screenheight/2;    twothree = 0;     twofour = (screenheight) / 2;
		threeone = 0;		    threetwo = 0;				threethree = 1;   threefour = 0;
		fourone = 0;		    fourtwo = 0;				fourthree = 0;    fourfour = 1;
	}
	void CreateOrthProjectionMatrix()
	{
		extern int screenwidth;
		extern int screenheight;
		extern float fn;
		extern float ff;

		float r, l, t, b, n, f;
		l = -screenwidth/2; r = screenwidth/2;
		t = screenheight/2; b = -screenwidth/2;
		//fn = 5; ff = 500;
		float ratio = ff / fn;

		Matrix4x4 moveone;
		Matrix4x4 scale;
		Matrix4x4 movetwo;

		/*moveone.oneone = 1;            moveone.onetwo = 0;		moveone.onethree = 0;         moveone.onefour = -oneone;
		moveone.twoone = 0;			   moveone.twotwo = 1;      moveone.twothree = 0;         moveone.twofour = -onetwo;
		moveone.threeone = 0;		   moveone.threetwo = 0;	moveone.threethree = 1;       moveone.threefour = -onethree;
		moveone.fourone = 0;		   moveone.fourtwo = 0;  	moveone.fourthree = 0;        moveone.fourfour = 1;*/

		oneone = 2 / (r - l);   onetwo = 0;					onethree = 0;               onefour = -(r + l) / (r - l);
		twoone = 0;				twotwo = 2 / (t - b);       twothree = 0;               twofour = -(t + b) / (t - b);
		threeone = 0;		    threetwo = 0;				threethree = 2 / (n - f);   threefour = -(n + f) / (n - f);
		fourone = 0;		    fourtwo = 0;				fourthree = 0;              fourfour = 1;

		/*oneone = 2/r;   onetwo = 0;					onethree = 0;                       onefour = -1;
		twoone = 0;				twotwo = 2 / t;       twothree = 0;                     twofour = -1;
		threeone = 0;		    threetwo = 0;				threethree = 2 / (n - f);   threefour = -(n + f) / (n - f);
		fourone = 0;		    fourtwo = 0;				fourthree = 0;              fourfour = 1;*/
	}
	void CreateCameraMatrix()//because everything is unit vector their values are between -1 and 1 just like cos and sin so thats why its rotation
	{
		//https://www.youtube.com/watch?v=RqZH-7hlI48 its a dot product which gets us new vector	
		//can just do matrix to put camera at its position then find inverse which would be our view matrix
		//https://www.youtube.com/watch?v=mpTl003EXCY

		extern int screenwidth;
		extern int screenheight;
		extern Vec3d cameralocation;
		extern Vec3d cameralookat;
		extern Vec3d cameraup;
		extern Vec3d cameraright;

		Matrix4x4 transform;
		Matrix4x4 rotate;

		transform.oneone = 1;				transform.onetwo = 0;			transform.onethree = 0;              transform.onefour = -cameralocation.x;
		transform.twoone = 0;				transform.twotwo = 1;			transform.twothree = 0;              transform.twofour = -cameralocation.y;
		transform.threeone = 0;		    transform.threetwo = 0;				transform.threethree = 1;			 transform.threefour = -cameralocation.z;
		transform.fourone = 0;		   transform.fourtwo = 0;				transform.fourthree = 0;             transform.fourfour = 1;

		rotate.oneone = cameraright.x;		    rotate.onetwo = cameraright.y;		 rotate.onethree = cameraright.z;		rotate.onefour = 0;
		rotate.twoone = cameraup.x;				rotate.twotwo = cameraup.y;			 rotate.twothree = cameraup.z;			rotate.twofour = 0;
		rotate.threeone = cameralookat.x;		rotate.threetwo = cameralookat.y;	 rotate.threethree = cameralookat.z;	rotate.threefour = 0;
		rotate.fourone = 0;						rotate.fourtwo = 0;				     rotate.fourthree = 0;					rotate.fourfour = 1;

		Matrix4x4 M;
		M = rotate * transform;

		*this = M;
	}
	void CreateProjectionMatrix()
	{
		extern int screenwidth;
		extern int screenheight;
		extern float ff;
		extern float fn;
		float n = fn;
		float f = ff;

		oneone = n;				onetwo = 0;					onethree = 0;               onefour = 0;
		twoone = 0;				twotwo = n;					twothree = 0;               twofour = 0;
		threeone = 0;		    threetwo = 0;				threethree = -(f+n)/(f-n);				threefour = -2*f*n/(f-n);  //// n + f   -f*n
		fourone = 0;		    fourtwo = 0;				fourthree = 1;              fourfour = 0;
	}
	void CreateLookatMatrix()
	{

		oneone = 0;				onetwo = 0;					onethree = 0;               onefour = 0;
		twoone = 0;				twotwo = 0;					twothree = 0;               twofour = 0;
		threeone = 0;		    threetwo = 0;				threethree = 1;				threefour = 0;  //// n + f   -f*n
		fourone = 0;		    fourtwo = 0;				fourthree = 1;              fourfour = 0;
	}
	/*this one seems to help with how x and y interact with z, the farther the z the less a movement changes the x and vice versa. based on proportions.*/
	void CreatePerspectiveMatrix()
	{
		extern int screenwidth;
		extern int screenheight;

		float n, f, y, ys, s;
		n = 1; f = 1000;
		y = screenwidth; ys = 0;

		float  ar, fov, r, l, t, b;
		ar = static_cast<float>(screenwidth) / static_cast<float>(screenheight);
		fov = 3;
		t = tan(fov / 2)*n;
		b = -t;
		r = t * ar;
		l = -t * ar;
		//cout << "left: " << l << " right: " << r << " top: " << t << " bot: " << b << endl;
		/*l = -screenwidth / 2; r = screenwidth/2;
		t = -screenheight / 2; b = screenheight/2;*/
		s = (1) / (tan(fov / 2 * (3.14 / 180)));

		/*oneone = (2 * n) / (r - l);				onetwo = 0;					onethree = (r+l)/(r-l);               onefour = 0;
		twoone = 0;				twotwo = (2*n)/(t-b);					twothree = (t+b)/(t-b);               twofour = 0;
		threeone = 0;		    threetwo = 0;				threethree = -(f+n)/(f-n);      threefour = -(2*f*n)/(f-n);  //// -f / (f - n)  //(-n * f) / (f - n)
		fourone = 0;		    fourtwo = 0;				fourthree = -1;              fourfour = 0;*/

		/*oneone = (2 * n) / screenwidth;				onetwo = 0;					onethree = -1;               onefour = 0;
		twoone = 0;				twotwo = (2*n)/screenheight;					twothree = -1;               twofour = 0;
		threeone = 0;		    threetwo = 0;				threethree = -(f + n) / (f - n);      threefour = -(2 * f*n) / (f - n);  //// -f / (f - n)  //(-n * f) / (f - n)
		fourone = 0;		    fourtwo = 0;				fourthree = 1;              fourfour = 0;*/

		oneone = n;				onetwo = 0;					onethree = 0;               onefour = 0;
		twoone = 0;				twotwo = n;					twothree = 0;               twofour = 0;
		threeone = 0;		    threetwo = 0;				threethree = 1;				threefour = 0;  //// n + f   -f*n
		fourone = 0;		    fourtwo = 0;				fourthree = 1;              fourfour = 0;
	}
	void CreateXRotationMatrix(float theta)
	{
		float angle = (theta * 180) / 3.14;

		oneone = 1;				onetwo = 0;					onethree = 0;               onefour = 0;
		twoone = 0;				twotwo = cos(angle);		twothree = -sin(angle);      twofour = 0;
		threeone = 0;		    threetwo = sin(angle);		threethree = cos(angle);    threefour = 0; 
		fourone = 0;		    fourtwo = 0;				fourthree = 0;              fourfour = 1;
	}
	void CreateYRotationMatrix(float theta)
	{
		float angle = (theta * 180) / 3.14;

		oneone = cos(angle);	  onetwo = 0;				onethree = -sin(angle);      onefour = 0;
		twoone = 0;				  twotwo = 1;				twothree = 0;               twofour = 0;
		threeone = sin(angle);   threetwo = 0;				threethree = cos(angle);    threefour = 0;
		fourone = 0;		      fourtwo = 0;				fourthree = 0;              fourfour = 1;
	}
	void CreateZRotationMatrix(float theta)
	{
		float angle = (theta * 180) / 3.14;

		oneone = cos(angle);	 onetwo = -sin(angle);		onethree = 0;               onefour = 0;
		twoone = sin(angle);	 twotwo = cos(angle);		twothree = 0;               twofour = 0;
		threeone = 0;			 threetwo = 0;				threethree = 1;             threefour = 0;
		fourone = 0;			 fourtwo = 0;				fourthree = 0;              fourfour = 1;
	}
	Vec4d CrossProduct(Matrix4x4 other)
	{

	}
	void CreateScaleMatrix(double scalex, double scaley, double scalez)
	{
		oneone = scalex;	     onetwo = 0;		        onethree = 0;               onefour = 0;
		twoone = 0;	              twotwo = scaley;		    twothree = 0;               twofour = 0;
		threeone = 0;			 threetwo = 0;				threethree = scalez;        threefour = 0;
		fourone = 0;			 fourtwo = 0;				fourthree = 0;              fourfour = 1;
	}
	void CreateYequalsBMatrix(int b)
	{
		
	}
	void CreateYequalsnegativeXMatrix()
	{
		
	}
	void CreateShearX(double x)
	{
	
	}
	void CreateShearY(double y)
	{
	
	}
	void CreateRotationMatrix(double degrees)
	{

	}
	void CreateTransformMatrix(double x, double y, double z)
	{
		oneone = 1;				onetwo = 0;					onethree = 0;               onefour = x;
		twoone = 0;				twotwo = 1;					twothree = 0;               twofour = y;
		threeone = 0;			 threetwo = 0;				threethree = 1;             threefour = z;
		fourone = 0;			 fourtwo = 0;				fourthree = 0;              fourfour = 1;
	}
	void PrintInfo()
	{
		cout << "[ " << oneone << " " << onetwo << " " << onethree << " " << onefour << " ]" << endl;
		cout << "[ " << twoone << " " << twotwo << " " << twothree << " " << twofour << " ]" << endl;
		cout << "[ " << threeone << " " << threetwo << " " << threethree << " " << threefour << " ]" << endl;
		cout << "[ " << fourone << " " << fourtwo << " " << fourthree << " " << fourfour << " ]" << endl << endl;
	}
	float oneone;
	float onetwo;
	float onethree;
	float onefour;
	float twoone;
	float twotwo;
	float twothree;
	float twofour;
	float threeone;
	float threetwo;
	float threethree;
	float threefour;
	float fourone;
	float fourtwo;
	float fourthree;
	float fourfour;

private:

};
