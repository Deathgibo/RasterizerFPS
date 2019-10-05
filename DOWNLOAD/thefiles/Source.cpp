#include <iostream>
#include <vector>
#include <stdio.h>    
#include <math.h>
#include <algorithm>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
#include "Vec3d.h"
#include "Matrix3x3.h"
#include "Triangle.h"
#include "Triangle3D.h"
#include "Matrix4x4.h"
#include "Vec4d.h"
#include "Mesh.h"
#include "Box.h"
#include "Circle.h"
#include "Ray.h"
#include "BSP.h"
#include <time.h>
using namespace std::chrono;
using namespace std;

struct bulletstruct {
	Mesh* mesh;
	Vec3d velocity;
	Vec3d originalmove;
	int lifespan = 0;
	float phi;
	float theta;
};

int screenwidth = 1000;
int screenheight = 550;
SDL_Window* gwindow = 0;
SDL_Renderer* grender = 0;
SDL_Surface* gsurface = 0;

/*Sounds*/
Mix_Music* akshoot = 0;
Mix_Chunk* akreload = 0;
Mix_Chunk* akshootchunk = 0;

/*Fonts*/
TTF_Font* gfont = 0;
SDL_Texture* ammocount = 0;
SDL_Texture* ammomax = 0;
int currentammo = 30;
int maxammo = 30;
int shootcounter = 0;

/*Frostum*/
float fl = -1;
float fr = 1;
float ft = -1;
float fb = 1;
float fn = 1;
float ff = 200;

Vec4d facenormals[6];
Vec4d facepoints[6];
vector<Triangle3D> clippedtriangles;
Vec4d axisright;

float lastmousex;
float lastmousey;
float mousechangex;
float mousechangey;
bool mousemoved = false;
bool nomousemove = false;

bool noclip = false;
bool wireframe = false;

Mesh SpaceShip;
Mesh Scarab;
Mesh Ghost;
Mesh MasterChief;
Mesh EnergySword;
Mesh CrashB;
Mesh thetriangle;
Mesh thesquare;
Mesh TeaPot;
Mesh Mountains;
Mesh videoship;
Mesh building;
Mesh ak47;
Mesh specialforces;
Mesh halogun;
Mesh Kirby;
Mesh Grunt;
Mesh bullet;
Mesh ivysaur;
Mesh circle1mesh;
Mesh sunmesh;
Mesh circle2mesh;
Mesh circle3mesh;
Mesh circle4mesh;
Mesh boxtarget1;
Mesh boxtarget2;
Mesh boxtarget3;
Mesh boxtarget4;
Mesh boxtarget5;
vector<bulletstruct> bullets;
Mesh BattleCreek;
vector<Triangle3D> boxtriangles;
vector<Triangle3D> boxtriangles2;
Box box2;
Box box3;
vector<Box> theboxes;
vector<Triangle3D> theboxestriangles;

vector<pointlight> thepointlights;

Vec3d crossproduct(Vec3d one, Vec3d two);
Vec3d gravity(0, .15, 0);
Vec3d cameralocation(0, 0, 0);
Vec3d cameralookat(0, 0, -1);
Vec3d cameraup(0, 1, 0);
Vec3d cameraright(1, 0, 0);
Vec4d cameralocation4d;
Vec3d cameralocation2(0, -10, -25);
Vec3d cameralookat2(0, 0, -1);
Vec3d cameraup2(0, 1, 0);
Vec3d cameraright2(1, 0, 0);
float pitch2 = 120.0f;
float yaw2 = 270.0f;
bool cameratwo = false;
Vec3d cameralocation3(0, 0, 0);
Vec3d cameralookat3(0, 0, -1);
Vec3d cameraup3(0, 1, 0);
Vec3d cameraright3(1, 0, 0);
float pitch3 = 120.0f;
float yaw3 = 270.0f;
bool camerathree = false;

float pitch = 90.0f;
float yaw = 270.0f;
vector<Triangle3D> allstatictriangles;
bool slow = false;
float akx = 0;
float aky = 0;
Vec3d originalxz(0, 0, -1);
Vec3d originalyz(0,1,0);
Vec4d cameraright4(1, 0, 0, 1);
Vec4d cameraup4(0, 1, 0, 1);
Vec4d cameralookat4(0, 0, -1, 1);
Triangle3D originalplayerbasis(cameraright4, cameraup4, cameralookat4);

float akcurrentx = 0;
float akcurrenty = 0;
bool firstakx = true;
Vec4d akinitialpositon;
Vec4d akinitialplayer;
bool justreleased = false;
float akphi;
float aktheta;
bool aiming = false;
bool pointlighton = true;
int number = 555;
int mousedelay = 0;

SDL_Texture* crosshair;

/*jump stuff*/
int jumpcounter = 0;
bool isjumping = false;
bool goingdown = false;
bool feetonground = false;

bool a = false;
bool d = false;
bool s = false;
bool w = false;

/*Circle*/
vector<Triangle3D> circle1tris;
vector<Mesh> targetmeshes;

/*menu options*/
bool menuon = false;
bool menu1 = true;
bool menu2 = false;
bool menu3 = false;
SDL_Texture* menuwords = 0;

/*Functions*/
bool checkcollision(Ray r, Triangle3D& tri);
float raytrihelper(int y, Ray r, Triangle3D& tri);
void LoadOBJ(Mesh& meshname, string filename);
void LoadSpaceShip(Mesh& meshname, string filename);
void LoadOBJVNT(Mesh& meshname, string filename, bool negativetroll = false);
void LoadOBJVT(Mesh& meshname, string filename);
void ClockWiseArc();
void EllipseBresenhamsMidpoint();
void CircleTrivial();
void CircleBresenhamsMidPoint();
void LineBresenhams();
Vec4d crossproduct(Vec4d one, Vec4d two);
void LineDDA();
bool init();
bool loadmedia();
SDL_Texture* loadtext(string s, SDL_Color textcolor);
SDL_Texture* loadimage(string a);
float toradians(float degrees);
void updatecamera();
void updatecamera2();
void updatecamera3(bool justx = false);
Vec3d normalize(Vec3d one);
void RenderedSortedTriangle();
void CalculateViewPlanes();
void CalculateViewPlanes2();
void CalculateViewPlanes3();
bool boxboxcollision(Box a, Box b);
void Jump();
bool playermoveback(Box boxa, Box boxb, Vec3d tmp);
int main(int argc, char* argv[])
{
	init();
	loadmedia();
	SDL_ShowCursor(SDL_DISABLE);
	bool exit = false;
	SDL_Event e;

	float xcounter = 0;
	float ycounter = 0;
	float zcounter = 0;

	lastmousex = screenwidth/2;
	lastmousey = screenheight/2;
	mousechangex = 0;
	mousechangey = 0;

	LoadSpaceShip(SpaceShip,"spaceship.txt");
	//LoadOBJ(Scarab, "scarab.txt");
	//LoadOBJ(Ghost, "ghost.txt");
	LoadOBJ(MasterChief, "masterchief.txt");
	LoadOBJ(EnergySword, "energysword.txt");
	LoadOBJ(CrashB, "crashb.txt");

	LoadSpaceShip(TeaPot, "teapot.txt");
	LoadSpaceShip(Mountains, "mountains.txt");
	LoadSpaceShip(videoship, "axis.txt");
	LoadSpaceShip(building, "globe.txt");
	//LoadOBJVNT(CrashB, "crashb.txt");
	LoadOBJVNT(ak47, "ak47.obj");
	LoadOBJVNT(specialforces, "Specialforce.obj");
	LoadOBJVNT(halogun, "ma37.obj");
	LoadOBJVNT(Kirby, "Kirby.obj");
	LoadOBJVT(Grunt, "Grunt.obj");
	LoadOBJVT(bullet, "bullet.obj");
	LoadOBJVT(ivysaur, "leon.obj");
	//LoadOBJVNT(BattleCreek,"BeaverCreek.obj",true);

	bullet.ReverseTriangles();
	bullet.SetNormals();
	bullet.SetColor(89, 89, 84);


	//BattleCreek.ReverseTriangles();
	//BattleCreek.SetNormals();
	BattleCreek.SetColor(100, 100, 100);
	BattleCreek.Scale(.01,.01,.01);

	Grunt.ReverseTriangles();
	Grunt.SetNormals();
	Grunt.SetColor(100, 149, 237);



	ak47.ReverseTriangles();
	ak47.SetNormals();
	ak47.SetColor(139, 69, 19);

	specialforces.ReverseTriangles();
	specialforces.SetNormals();
	specialforces.SetColor(100, 100, 100);

	halogun.ReverseTriangles();
	halogun.SetNormals();
	halogun.SetColor(10, 10, 10);

	//CrashB.ReverseNormals();
	CrashB.ReverseTriangles();
	SpaceShip.ReverseTriangles();
	MasterChief.ReverseTriangles();
	CrashB.SetNormals();
	SpaceShip.SetNormals();
	MasterChief.SetNormals();
	TeaPot.ReverseTriangles();
	TeaPot.SetNormals();
	Mountains.ReverseTriangles();
	Mountains.SetNormals();
	Scarab.SetNormals();
	EnergySword.SetNormals();

	//building.ReverseTriangles();
	building.SetNormals();

	videoship.ReverseTriangles();
	videoship.SetNormals();

	Triangle triangle(400, 300, 1, 500, 300, 1, 450, 200, 1);

	/*3D triangle*/
	/*Vec4d left(200, 300, 10, 1);
	Vec4d right(300, 300, 10, 1);
	Vec4d top(250, 200, 10, 1);
	Vec4d back(250, 300, 50, 1);
	Triangle3D t1(left,right,top);
	Triangle3D t2(top,right,back);
	Triangle3D t3(top,left,back);
	Triangle3D t4(left, right, back);
	Mesh thetriangle;
	thetriangle.AddTriangle(t1); thetriangle.AddTriangle(t2); thetriangle.AddTriangle(t3); thetriangle.AddTriangle(t4);
	thetriangle.Projection();*/

	Vec4d left(-.2 + xcounter, .2 + ycounter, 1 + zcounter, 1);
	Vec4d right(.2 + xcounter, .2 + ycounter, 1 + zcounter, 1);
	Vec4d top(0.0f + xcounter, -.2 + ycounter, 1 + zcounter, 1);
	Vec4d back(0.0f + xcounter, .2 + ycounter, 1.2 + zcounter, 1);
	Vec4d tricenter(0, 0, 1.1, 1);
	/*Vec4d left(200 + xcounter, 300 + ycounter, 1 + zcounter, 1);
	Vec4d right(300 + xcounter, 300 + ycounter, 1 + zcounter, 1);
	Vec4d top(250 + xcounter, 200 + ycounter, 1 + zcounter, 1);
	Vec4d back(250 + xcounter, 300 + ycounter, -100 + zcounter, 1);*/
	//clockwise
	Triangle3D t1(top, right, left);
	Triangle3D t2(top, back, right);
	Triangle3D t3(top, left, back);
	Triangle3D t4(right, back, left);
	
	thetriangle.AddTriangle(t1); thetriangle.AddTriangle(t2); thetriangle.AddTriangle(t3); thetriangle.AddTriangle(t4);

	thetriangle.SetNormals();

	Vec4d fronttopleft(.8 + xcounter, -1.5 + ycounter, 1 + zcounter, 1);
	Vec4d fronttopright(13 + xcounter, -1.5 + ycounter, 1 + zcounter, 1);
	Vec4d frontbottomleft(.8 + xcounter, -.3 + ycounter, 1 + zcounter, 1);
	Vec4d frontbottomright(13 + xcounter, -.3 + ycounter, 1 + zcounter, 1);
	Vec4d backtopleft(.8 + xcounter, -1.5 + ycounter, 5.5 + zcounter, 1);
	Vec4d backtopright(13 + xcounter, -1.5 + ycounter, 5.5 + zcounter, 1);
	Vec4d backbottomleft(.8 + xcounter, -.3 + ycounter, 5.5 + zcounter, 1);
	Vec4d backbottomright(13 + xcounter, -.3 + ycounter, 5.5 + zcounter, 1);
	Vec4d squarecenter(.9, -.4, 1.025, 1);
	/*Vec4d fronttopleft(500 + xcounter, 500 + ycounter, 100 + zcounter, 1);
		Vec4d fronttopright(600 + xcounter, 500 + ycounter, 100 + zcounter, 1);
		Vec4d frontbottomleft(500 + xcounter, 600 + ycounter, 100 + zcounter, 1);
		Vec4d frontbottomright(600 + xcounter, 600 + ycounter, 100 + zcounter, 1);
		Vec4d backtopleft(500 + xcounter, 500 + ycounter, 150 + zcounter, 1);
		Vec4d backtopright(600 + xcounter, 500 + ycounter, 150 + zcounter, 1);
		Vec4d backbottomleft(500 + xcounter, 600 + ycounter, 150 + zcounter, 1);
		Vec4d backbottomright(600 + xcounter, 600 + ycounter, 150 + zcounter, 1);*/
		//front
	Triangle3D s1(fronttopleft, fronttopright, frontbottomleft);
	Triangle3D s2(fronttopright, frontbottomright, frontbottomleft);
	//right
	Triangle3D s3(fronttopright, backbottomright, frontbottomright);
	Triangle3D s4(backtopright, backbottomright, fronttopright);
	//left
	Triangle3D s5(fronttopleft, frontbottomleft, backbottomleft);
	Triangle3D s6(backtopleft, fronttopleft, backbottomleft);
	//back
	Triangle3D s7(backtopleft, backbottomleft, backtopright);
	Triangle3D s8(backtopright, backbottomleft, backbottomright);
	//top
	Triangle3D s9(fronttopleft, backtopright, fronttopright);
	Triangle3D s10(backtopleft, backtopright, fronttopleft);
	//bottom
	Triangle3D s11(frontbottomleft, frontbottomright, backbottomleft);
	Triangle3D s12(backbottomleft, frontbottomright, backbottomright);

	thesquare.AddTriangle(s1); thesquare.AddTriangle(s2); thesquare.AddTriangle(s3); thesquare.AddTriangle(s4);
	thesquare.AddTriangle(s5); thesquare.AddTriangle(s6); thesquare.AddTriangle(s7); thesquare.AddTriangle(s8);
	thesquare.AddTriangle(s9); thesquare.AddTriangle(s10); thesquare.AddTriangle(s11); thesquare.AddTriangle(s12);
	thesquare.SetNormals();
	thesquare.Scale(15, 2, 15);
	thesquare.Move(-30, 10, -20);

	//EnergySword.Scale(.05, .05, .05);
	EnergySword.Rotate(.03, 0, .19, EnergySword.Triangles[0].one); //.064, 5
	EnergySword.Move(1, 1,-1.2);
	//targetmeshes.push_back(EnergySword);

	
	Vec4d axisright2(1, 0, 0, 1);
	Vec4d axisforward(0, 1, 0, 1);
	Vec4d axisz(0, 0, 1, 1);

	Vec4d cheifcenter(0, 0, 0, 1);
	MasterChief.RodriguesRotation(axisright2, 1.57);
	MasterChief.RodriguesRotation(axisforward, 1.57);
	MasterChief.Scale(.04, .04, .04);
	MasterChief.SetColor(10, 155, 10);
	MasterChief.Move(0, 6, 0);
	//targetmeshes.push_back(MasterChief)

	CrashB.Scale(.02, .02, .02);
	CrashB.RodriguesRotation(axisz, 3.14);
	CrashB.RodriguesRotation(axisforward, -1.57);
	CrashB.Move(0, 5.5, 7);
	CrashB.SetColor(100, 100, 100);
	targetmeshes.push_back(CrashB);


	TeaPot.Rotate(.6, 0, 0, cheifcenter);
	TeaPot.Move(10, 0, 0);
	TeaPot.Scale(2, 2, 2);
	TeaPot.SetColor(100, 100, 100);

	//SpaceShip.Scale(2, 2, 2);
	//SpaceShip.Rotate(0, 0.02010, 0, tricenter);
	//targetmeshes.push_back(SpaceShip);
	
	Vec4d mountaincenter(0, 0, 0, 1);
	Mountains.Scale(.7, .7, .7);
	Mountains.Rotate(.6, 0, 0, mountaincenter);
	
	specialforces.Scale(.01, .01, .01);
	specialforces.Rotate(.6, 0, 0, mountaincenter);
	specialforces.Move(0, 5, 0);

	halogun.Scale(.001, .001, .001);
	specialforces.Rotate(.6, 0, 0, mountaincenter);
	specialforces.Move(0, 6, 0);

	Grunt.Scale(.03, .03, .03);
	Grunt.RodriguesRotation(axisforward, -1.57);
	Grunt.RodriguesRotation(axisz, 3.14);
	Grunt.Move(0, 1.4, -.5);

	Kirby.ReverseTriangles();
	Kirby.SetNormals();
	Kirby.SetColor(155, 100, 100);
	Kirby.Scale(12, 12, 12);
	Kirby.RodriguesRotation(axisz, 3.14);
	Kirby.RodriguesRotation(axisforward, 1.57);
	Kirby.Move(0, 5, 7);
	//targetmeshes.push_back(Kirby);

	ivysaur.ReverseTriangles();
	ivysaur.SetNormals();
	ivysaur.SetColor(20, 150, 100);
	ivysaur.Scale(.1, .1, .1);
	ivysaur.RodriguesRotation(axisz, 3.14);
	//targetmeshes.push_back(ivysaur);

	ak47.Scale(2, 2, 2);
	ak47.RodriguesRotation(axisright2, 1.57);
	vector<Triangle3D> akoriginal;
	akoriginal = ak47.Triangles;


	akinitialpositon = ak47.Triangles[0].one;
	ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
	Vec3d position2 = (cameraright * .2) + (cameraup * .2) + (cameralookat * -.5);
	ak47.Move(position2.x,position2.y,position2.z);//.2
	akinitialplayer = ak47.Triangles[0].one;

	bullet.Scale(.04, .04, .04);
	bullet.RodriguesRotation(axisright2, -1.57);
	bullet.RodriguesRotation(axisforward, -3.14);

	/*Box 1*/
	Box box1;
	Vec4d boxcenter(-8, 3, -5, 1);
	box1.CreateBox(boxcenter, 2, 10, 155, 155);
	boxtriangles = box1.Converttotriangles();
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*Box 2*/
	Vec4d boxcenter3(0, 4, 2, 1);
	box1.CreateBox(boxcenter3, 1, 50, 155, 155);
	boxtriangles = box1.Converttotriangles();
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*light box */
	Vec4d boxcenter6(3, 2, .2, 1);
	box1.CreateBox(boxcenter6, .2, 50, 155, 155);
	boxtriangles = box1.Converttotriangles();
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*Rectanglebox 3*/
	Vec4d boxcenter4(3.5, 3, 6, 1);
	box1.CreateRectangle(boxcenter4, 2,2,5,50,155,155);
	boxtriangles = box1.ConverttoSmallerTriangles(3);
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*flat 1*/
	Vec4d boxcenter8(-3.5, 0, 8, 1);
	box1.CreateRectangle(boxcenter8, 2, .2, 2, 50, 110, 178);
	boxtriangles = box1.ConverttoSmallerTriangles(0);
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*flat 2*/
	Vec4d boxcenter9(-9.5, 0, 2, 1);
	box1.CreateRectangle(boxcenter9, 1.6, .2, 1.6, 150, 50, 78);
	boxtriangles = box1.ConverttoSmallerTriangles(0);
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		theboxestriangles.push_back(boxtriangles[i]);
	}
	boxtriangles.clear();
	/*target 1*/
	Vec4d boxcenter10(0, 0, 0, 1);
	box1.CreateRectangle(boxcenter10, .4, .4, .4, 100, 100, 100);
	boxtriangles = box1.ConverttoSmallerTriangles(1);
	//theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtarget1.AddTriangle(boxtriangles[i]);
	}
	boxtarget1.SetNormals();
	boxtarget1.SetColor(100, 100, 100);
	boxtarget1.Move(-5.5, 4, -5);
	targetmeshes.push_back(boxtarget1);
	/*target 2*/
	Vec4d boxcenter11(0, 0, 0, 1);
	box1.CreateRectangle(boxcenter11, .4, .4, .4, 100, 100, 100);
	boxtriangles = box1.ConverttoSmallerTriangles(1);
	//theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtarget2.AddTriangle(boxtriangles[i]);
	}
	boxtarget2.SetNormals();
	boxtarget2.SetColor(100, 100, 100);
	boxtarget2.Move(-5.5, 2.5, -6);
	targetmeshes.push_back(boxtarget2);
	/*target 3*/
	Vec4d boxcenter12(0, 0, 0, 1);
	box1.CreateRectangle(boxcenter12, .4, .4, .4, 100, 100, 100);
	boxtriangles = box1.ConverttoSmallerTriangles(1);
	//theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtarget3.AddTriangle(boxtriangles[i]);
	}
	boxtarget3.SetNormals();
	boxtarget3.SetColor(100, 100, 100);
	boxtarget3.Move(-5.5, 2.5, -4);
	targetmeshes.push_back(boxtarget3);
	/*target 4*/
	Vec4d boxcenter13(0, 0, 0, 1);
	box1.CreateRectangle(boxcenter13, 1, 1, 1, 100, 100, 100);
	boxtriangles = box1.ConverttoSmallerTriangles(2);
	//theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtarget4.AddTriangle(boxtriangles[i]);
	}
	boxtarget4.SetNormals();
	boxtarget4.SetColor(100, 100, 100);
	boxtarget4.Move(-9, 2.5, 10);
	targetmeshes.push_back(boxtarget4);
	/*target 5*/
	Vec4d boxcenter14(0, 0, 0, 1);
	box1.CreateRectangle(boxcenter14, 1.5, 1.5, 1.5, 100, 100, 100);
	boxtriangles = box1.ConverttoSmallerTriangles(2);
	//theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtarget5.AddTriangle(boxtriangles[i]);
	}
	boxtarget5.SetNormals();
	boxtarget5.SetColor(100, 100, 100);
	boxtarget5.Move(10, 2.5, -10);
	targetmeshes.push_back(boxtarget5);
	/*Box floor*/
	Vec4d boxcenter5(0, 10, 0, 1);
	box1.CreateRectangle(boxcenter5, 15,5,15, 155, 155, 155);
	boxtriangles = box1.ConverttoSmallerTriangles(7,true);
	theboxes.push_back(box1);
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		//theboxestriangles.push_back(boxtriangles[i]);
	}
	//boxtriangles.clear();


	/*player box*/
	Vec4d boxcenter2(cameralocation.x, cameralocation.y, cameralocation.z, 1);
	//box2.CreateBox(boxcenter2, .4);
	box2.CreateRectangle(boxcenter2, .4, 1, .4, 0, 155, 155);

	/*POINT LIGHTS*/
	pointlight one;
	Vec4d lightposition(-3, -2, 1.5, 1); one.lightposition = lightposition;
	Vec4d lightcolor(1.0f, 1.0f, 1.0f, 1); one.lightcolor = lightcolor;
	one.constant = 1; one.exponent = .7; one.intensity = 2; one.top = 1;// one.backward = 1; 
	
	pointlight two;
	Vec4d lightposition2(8, -4, 0, 1); two.lightposition = lightposition2;
	Vec4d lightcolor2(0.0f, 1.0f, 0.0f, 1); two.lightcolor = lightcolor2;
	two.constant = 1; two.exponent = .5; two.intensity = 2; //one.top = 1;// one.backward = 1; 

	pointlight pthree;
	Vec4d lightposition3(2, -4, -8, 1); pthree.lightposition = lightposition3;
	Vec4d lightcolor3(1.0f, 0.0f, 0.0f, 1); pthree.lightcolor = lightcolor3;
	pthree.constant = 1; pthree.exponent = .7; pthree.intensity = 1; //one.top = 1;// one.backward = 1; 

	/*push back*/
	thepointlights.push_back(one);
	thepointlights.push_back(two);
	thepointlights.push_back(pthree);

	/*CIRCLES*/
	Circle sun;
	Vec4d suncenter(0, 0, 0, 1);
	sun.CreateCircle(suncenter, .8 , 255, 255, 0);
	circle1tris = sun.ConverttoSmallerTriangles(40);
	for (int i = 0; i < circle1tris.size(); i++)
	{
		sunmesh.AddTriangle(circle1tris[i]);
	}
	sunmesh.RodriguesRotation(axisforward, -1.57);
	sunmesh.SetColor(100, 100, 100);
	sunmesh.Move(-20.5, 0, 0);

	Circle target4;
	Vec4d target4center(0, 0, 0, 1);
	target4.CreateCircle(target4center, .8, 255, 255, 0);
	circle1tris = target4.ConverttoSmallerTriangles(40);
	for (int i = 0; i < circle1tris.size(); i++)
	{
		circle4mesh.AddTriangle(circle1tris[i]);
	}
	circle4mesh.RodriguesRotation(axisforward, -1.57);
	circle4mesh.SetColor(100, 100, 100);
	circle4mesh.Move(-20.5, 0, 5);

	Circle target2;
	Vec4d target2center(0, 0, 0, 1);
	target2.CreateCircle(target2center, 1.4, 255, 255, 0);
	circle1tris = target2.ConverttoSmallerTriangles(20);
	for (int i = 0; i < circle1tris.size(); i++)
	{
		circle2mesh.AddTriangle(circle1tris[i]);
	}
	circle2mesh.RodriguesRotation(axisforward, 1.57);
	circle2mesh.SetColor(100, 100, 100);
	circle2mesh.Move(4, -1, 7);


	Circle target3;
	Vec4d target3center(0, 0, 0, 1);
	target3.CreateCircle(target3center, 15, 255, 255, 0);
	circle1tris = target3.ConverttoSmallerTriangles(10);
	for (int i = 0; i < circle1tris.size(); i++)
	{
		circle3mesh.AddTriangle(circle1tris[i]);
	}
	//circle3mesh.RodriguesRotation(axisforward, 1.57);
	circle3mesh.SetColor(100, 100, 100);
	circle3mesh.Move(0, -10, 50);

	Circle circle1;
	Vec4d circlecenter(0, 0, 0, 1);
	circle1.CreateCircle(circlecenter, 2, 100, 100, 100);
	circle1tris = circle1.ConverttoSmallerTriangles(20);
	for (int i = 0; i < circle1tris.size(); i++)
	{
		circle1mesh.AddTriangle(circle1tris[i]);
	}
	circle1mesh.RodriguesRotation(axisforward, 1.57);
	circle1mesh.SetColor(100, 100, 100);
	circle1mesh.Move(20, 0, 0);

	/*Target meshes*/
	targetmeshes.push_back(circle1mesh);
	targetmeshes.push_back(circle2mesh);
	targetmeshes.push_back(circle3mesh);
	targetmeshes.push_back(circle4mesh);
	targetmeshes.push_back(sunmesh);

	updatecamera2();
	updatecamera3();
	for (Triangle3D x : thetriangle.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : thesquare.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : SpaceShip.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : CrashB.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : MasterChief.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : TeaPot.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : Mountains.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : videoship.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : EnergySword.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : building.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : ak47.Triangles)
	{
		//allstatictriangles.push_back(x);
	}
	for (Triangle3D x : theboxestriangles)
	{
		//allstatictriangles.push_back(x);
	}

	BSP bsp1;
	auto startbsp = high_resolution_clock::now();
	//bsp1.CreateTree(allstatictriangles);
	auto stopbsp = high_resolution_clock::now();
	auto durationbsp = duration_cast<microseconds>(stopbsp - startbsp);
	//cout << "bsp creation time: " << durationbsp.count() << "ms size: " << bsp1.Sizestart() << endl;

	float fps = 0;
	Vec4d playercenter(0, 0, 0, 1);
	float cameraspeed = 1.0f;
	int acounter = 0;
	bool shooting = false;
	bool reloading = false;
	bool pressreload = false;
	bool reloadanimationonce = false;
	bool justreloaded = false;

	while (exit == false)
	{
		auto start = high_resolution_clock::now();
		mousechangex = 0;
		mousechangey = 0;
		akx = 0;
		aky = 0;
		justreleased = false;
		mousemoved = false;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				exit = true;
			}
			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					cout << "yikes"; break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					cout << "sup"; break;
				default: break;
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				Vec4d location(cameralocation.x, cameralocation.y, cameralocation.z, 1);
				float xamount, yamount, zamount;
				float amount = .001;
				xamount = .1;
				yamount = .1;
				zamount = .1;
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN: if (menuon && menu1) { menu3 = true; menu2 = false; menu1 = false; } if (menuon && menu2) { exit = true; };	break;
				case SDLK_BACKSPACE: if (menuon && menu3) { menu1 = true; menu2 = false; menu3 = false; }; break;
				case SDLK_s: break;//cameralocation = cameralocation - cameralookat; break;// zcounter += zamount;     thetriangle.add(0, 0, zamount);	thesquare.add(0, 0, zamount);	SpaceShip.add(0, 0, zamount);	MasterChief.add(0, 0, zamount);	 EnergySword.add(0, 0, zamount);	CrashB.add(0, 0, zamount);	tricenter.z += zamount; squarecenter.z += zamount;  break;
				case SDLK_w: break;//cameralocation = cameralocation + cameralookat; break;//zcounter -= zamount;     thetriangle.add(0, 0, -zamount);	thesquare.add(0, 0, -zamount);	SpaceShip.add(0, 0, -zamount);	MasterChief.add(0, 0, -zamount);  EnergySword.add(0, 0, -zamount);  CrashB.add(0, 0, -zamount); tricenter.z += -zamount; squarecenter.z += -zamount; break;
				case SDLK_a:break;// cameralocation = cameralocation - cameraright; break;// xcounter -= xamount;     thetriangle.add(-xamount, 0, 0);	thesquare.add(-xamount, 0, 0);	SpaceShip.add(-xamount, 0, 0);	MasterChief.add(-xamount, 0, 0);  EnergySword.add(-xamount, 0, 0); CrashB.add(-xamount, 0, 0); tricenter.x += -xamount; squarecenter.x += -xamount; break;
				case SDLK_d: break;//cameralocation = cameralocation + cameraright; break;// xcounter += xamount;     thetriangle.add(xamount, 0, 0);	thesquare.add(xamount, 0, 0);	SpaceShip.add(xamount, 0, 0);	MasterChief.add(xamount, 0, 0);	 EnergySword.add(xamount, 0, 0); 	CrashB.add(xamount, 0, 0); tricenter.x += xamount; squarecenter.x += xamount;  break;
				case SDLK_SPACE: if(feetonground) isjumping = true; break;//cameralocation = cameralocation - cameraup; break; //ycounter -= yamount; thetriangle.add(0, -yamount, 0);  thesquare.add(0, -yamount, 0);  SpaceShip.add(0, -yamount, 0);  MasterChief.add(0, -yamount, 0);  EnergySword.add(0, -yamount, 0); CrashB.add(0, -yamount, 0); tricenter.y += -yamount; squarecenter.y += -yamount; break;
				case SDLK_LCTRL: if (noclip) break; box2.CreateRectangle(location, .4, .5, .4, 0, 155, 155); break; //cameralocation = cameralocation + cameraup; break;// ycounter += yamount; thetriangle.add(0, yamount, 0);	thesquare.add(0, yamount, 0);	SpaceShip.add(0, yamount, 0);	MasterChief.add(0, yamount, 0);  EnergySword.add(0, yamount, 0);   CrashB.add(0, yamount, 0);  tricenter.y += yamount; squarecenter.y += yamount;  break;
				case SDLK_LEFT:   ; break;//yaw += cameraspeed; 		updatecamera(); break;//SpaceShip.Rotate(0, amount, 0, playercenter); thetriangle.Rotate(0, amount, 0, playercenter); thesquare.Rotate(0, amount, 0, playercenter); MasterChief.Rotate(0, amount, 0, playercenter);     EnergySword.Rotate(0, amount, 0, playercenter); CrashB.Rotate(0, amount, 0, playercenter);   break;
				case SDLK_RIGHT:  ; break; //yaw -= cameraspeed; updatecamera(); break;//SpaceShip.Rotate(0, -amount, 0, playercenter); thetriangle.Rotate(0, -amount, 0, playercenter); thesquare.Rotate(0, -amount, 0, playercenter); MasterChief.Rotate(0, -amount, 0, playercenter); EnergySword.Rotate(0, -amount, 0, playercenter); CrashB.Rotate(0, -amount, 0, playercenter);  break;
				case SDLK_UP:     if (menuon && menu2) { menu1 = true; menu2 = false; menu3 = false; }; break;//pitch += cameraspeed; updatecamera(); break;//SpaceShip.Rotate(amount, 0, 0, playercenter); thetriangle.Rotate(amount, 0, 0, playercenter); thesquare.Rotate(amount, 0, 0, playercenter); MasterChief.Rotate(amount, 0, 0, playercenter);      EnergySword.Rotate(amount, 0, 0, playercenter); CrashB.Rotate(amount, 0, 0, playercenter);   break;
				case SDLK_DOWN:   if (menuon && menu1) { menu2 = true; menu1 = false; menu3 = false; } break;//pitch -= cameraspeed; updatecamera(); break;//SpaceShip.Rotate(-amount, 0, 0, playercenter); thetriangle.Rotate(-amount, 0, 0, playercenter); thesquare.Rotate(-amount, 0, 0, playercenter); MasterChief.Rotate(-amount, 0, 0, playercenter);  EnergySword.Rotate(-amount, 0, 0, playercenter); CrashB.Rotate(-amount, 0, 0, playercenter);  break;
				case SDLK_ESCAPE:  menuon = !menuon; menu1 = true; menu2 = false; menu3 = false; break;//exit= true
				case SDLK_BACKQUOTE: noclip = !noclip; break;
				case SDLK_MINUS: wireframe = !wireframe; break;
				case SDLK_p: pointlighton = !pointlighton; break;
				case SDLK_1: cameratwo = !cameratwo; camerathree = false; break;
				case SDLK_2: camerathree = !camerathree; cameratwo = false; break;
				case SDLK_r: if(currentammo != maxammo) pressreload = true; break;
				case SDLK_n: number++; break;
				case SDLK_b: slow = !slow; break;
				case SDLK_8: maxammo -= 10; break;
				case SDLK_9: maxammo += 10; break;
				case SDLK_m: nomousemove = !nomousemove; break;
				case SDLK_g:
					for (int i = 0; i < targetmeshes.size(); i++)
					{
						for (int t = 0; t < targetmeshes[i].Triangles.size(); t++)
						{
							targetmeshes[i].Triangles[t].one.r = 100;
							targetmeshes[i].Triangles[t].one.g = 100;
							targetmeshes[i].Triangles[t].one.b = 100;
						}
					}
					break;
				}
				thepointlights[0].lightposition = lightposition; thepointlights[0].lightposition.w = 1;
			}
			else if (e.type == SDL_KEYUP)
			{
				Vec4d location(cameralocation.x, cameralocation.y, cameralocation.z, 1);

				switch (e.key.keysym.sym)
				{
				case SDLK_LCTRL:
					if (noclip) break;
					Vec3d tmp(0, -.5, 0);
					cameralocation = cameralocation + tmp;
					ak47.Move(tmp.x, tmp.y, tmp.z);
					Grunt.Move(tmp.x, tmp.y, tmp.z);
					box2.Move(tmp.x, tmp.y, tmp.z);
					for (int i = 0; i < theboxes.size(); i++)
					{
						playermoveback(box2, theboxes[i], tmp);
					}
					Vec4d boxcenter2(cameralocation.x, cameralocation.y, cameralocation.z, 1);
					box2.CreateRectangle(boxcenter2, .4, 1, .4, 0, 155, 155);
					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				mousemoved = true;
				int x, y;
				SDL_GetMouseState(&x, &y);

				mousechangey = y - (screenheight/2);
				mousechangex = x - (screenwidth / 2);
			//	cout << "mouse change: " << mousechangex << " " << mousechangey << endl;

				lastmousex = x;
				lastmousey = y;

			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_RIGHT && currentammo != 0 )
				{
					reloading = false;
					pressreload = false;
					Mix_HaltChannel(1);
					aiming = true;
					//cout << "aim" << endl;
					//transform to normal basis, move, then transform back
					//just have a center and find how center gets to player
					ak47.Triangles = akoriginal;

					
					justreleased = true;

					ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
					//ak47.Move(-(ak47.Triangles[0].one.x - cameralocation.x), -(ak47.Triangles[0].one.y - cameralocation.y), -(ak47.Triangles[0].one.z - cameralocation.z));
					ak47.Move(0,.1,.2);

					
				}
				if (e.button.button == SDL_BUTTON_LEFT && currentammo != 0 && !shooting)
				{
					Mix_PlayMusic(akshoot, 1);
					shooting = true;
					pressreload = false;
					reloading = false;
					Mix_HaltChannel(1);
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_RIGHT && currentammo != 0 && !reloading)
				{
					aiming = false;
					//ak47.Move(.2*cameraright.x, .2*cameraup.y, -.5*cameralookat.z);//.2
					//cout << "release" << endl;
					ak47.Triangles = akoriginal;

					//ak47.Move(-(ak47.Triangles[0].one.x - akinitialpositon.x), -(ak47.Triangles[0].one.y - akinitialpositon.y), -(ak47.Triangles[0].one.z - akinitialpositon.z));
					//cout << ak47.Triangles[0].one.x << endl;
					//ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
					Vec3d position;
					ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);

					position = (cameraright * -.2) + (cameraup * -.2) + (cameralookat * -.5);
					//ak47.Move(position.x, position.y, position.z);


					ak47.Move(.2, .2, .5);

	
					justreleased = true;
				}
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (Mix_PlayingMusic())
					{
						Mix_PlayChannel(0, akshootchunk, 0);
					}
					Mix_HaltMusic();
					shooting = false;
				}
			}

		}
		
	/*				Main Loop						*/
		
		const Uint8* currentkeystate = SDL_GetKeyboardState(0);
		float turnspeed = 0.3f;
		if (mousedelay < 1)
		{
			mousechangex = 0;
			mousechangey = 0;
		}
		yaw -= mousechangex * turnspeed;
		pitch += mousechangey * turnspeed;
		updatecamera();
		if (camerathree)
		{
			updatecamera3();
			cameralocation3 = cameralocation + cameralookat*2 + cameraup*1;
		}
		//find angle between xy components of lookat and originalxz player is the center
		//normalized so length is always 1
		//spherical coordinates
		float sphereradius = sqrt(pow(cameralookat.x, 2) + pow(cameralookat.y, 2) + pow(cameralookat.z, 2));
		float phi = 0;
		float theta = 0;
		phi = acos((cameralookat.y) / sphereradius);
		if (phi < .01)
		{
			phi = .01;
		}

		float value = (cameralookat.x) / (sphereradius*sin(phi));

		if (value < -1)
		{
			value = -1;
		}
		if (value > 1)
		{
			value = 1;
		}
		theta = acos(value);
		if (cameralookat.z > 0)
		{
			theta = (3.14 * 2) - theta;
		}

		float angle1;
		float distance = sqrt(pow(cameralookat.x, 2) + pow(cameralookat.z, 2));
		angle1 = acos(cameralookat.x/distance);//center is 0,0 and radius is 1 so can ignore those
		if(cameralookat.z < 0)
		{
			angle1 = (3.14*2) - angle1;
		}
		Vec4d basis1(0,0,1,1);
		Vec4d center(0, 0, 0, 1);
		//basis1.RotateX(angle1, center);
		
		


		if (firstakx)
		{
			akcurrentx = theta;
			akcurrenty = phi;
			//aktheta += theta;
			//akphi += phi;
			firstakx = false;
		}

		//cout <<"xz angle: "<< (angle1*180)/3.14 << endl;
		Vec4d axisup(cameraup.x, cameraup.y, cameraup.z, 1);
		//Vec4d axisright(cameraright.x, cameraright.y, cameraright.z, 1);
		
		Vec4d dummy(0, 1, 0, 1);
		//dummy = dummy.normalize();

		/*GLITCH DEALS WITH CODE HERE*/

		ak47.Move(-cameralocation.x, -cameralocation.y, -cameralocation.z);
		Grunt.Move(-cameralocation.x, -cameralocation.y, -cameralocation.z);
		if (justreleased || justreloaded)
		{
			ak47.RodriguesRotation(dummy, aktheta);
			ak47.RodriguesRotation(axisright, akphi);
			justreloaded = false;
		}
		
		axisright.x = cameraright.x; axisright.y = cameraright.y; axisright.z = cameraright.z; axisright.w = 1;
		
		ak47.RodriguesRotation(dummy, theta - akcurrentx);
		ak47.RodriguesRotation(axisright, phi - akcurrenty);
		Grunt.RodriguesRotation(dummy, theta - akcurrentx);

		
		originalplayerbasis.Rodriguesrotate(dummy, originalplayerbasis.one,theta - akcurrentx);
		originalplayerbasis.Rodriguesrotate(dummy, originalplayerbasis.two, theta - akcurrentx);
		originalplayerbasis.Rodriguesrotate(dummy, originalplayerbasis.three, theta - akcurrentx);
		ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
		Grunt.Move(cameralocation.x, cameralocation.y, cameralocation.z);
		aktheta += (theta - akcurrentx);
		akphi += (phi - akcurrenty);
		akcurrentx = theta;
		akcurrenty = phi;

		int x, y;
		SDL_GetMouseState(&x, &y);

		if (!nomousemove)
		{
			SDL_ShowCursor(SDL_DISABLE);
			SDL_WarpMouseInWindow(gwindow, (screenwidth / 2), (screenheight / 2));
		}
		else
		{
			SDL_ShowCursor(SDL_ENABLE);
		}

		float movementspeed = 0.1f;
		Vec3d thetmp(0, 0, 0);
		Vec3d originalcameralocation = cameralocation;
		int axis = 0;//1 is x 2 is y 3 is z
		if (!noclip)//gravity
		{
			cameralocation = cameralocation + gravity;
			ak47.Move(gravity.x, gravity.y, gravity.z);
			Grunt.Move(gravity.x, gravity.y, gravity.z);
			box2.Move(gravity.x, gravity.y, gravity.z);
			thetmp = gravity;

			bool once = false;
			for (int i = 0; i < theboxes.size(); i++)
			{
				if (playermoveback(box2, theboxes[i], gravity))
				{
					once = true;
				}
			}
			feetonground = once;
		}
		if (isjumping && !noclip)
		{
			Jump();
		}
		Vec4d left(-1, 0, 0, 1);
		Vec4d right(1, 0, 0, 1);
		Vec4d forward(0, 0, -1, 1);
		Vec4d backware(0, 0, 1, 1);

		a = false;
		d = false;
		s = false;
		w = false;

		if (currentkeystate[SDL_SCANCODE_A])
		{
			a = true;
			if (noclip)//gravity
			{
				Vec3d tmp(cameraright.x*movementspeed, cameraright.y*movementspeed, cameraright.z*movementspeed);
				cameralocation = cameralocation + tmp;
				ak47.Move(tmp.x, tmp.y, tmp.z);
				Grunt.Move(tmp.x, tmp.y, tmp.z);
				box2.Move(tmp.x, tmp.y, tmp.z);
				thetmp = tmp;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
			else
			{
				Vec3d tmp(-originalplayerbasis.one.x*movementspeed, 0, -originalplayerbasis.one.z*movementspeed);
				cameralocation = cameralocation + tmp;
				ak47.Move(tmp.x, tmp.y, tmp.z);
				Grunt.Move(tmp.x, tmp.y, tmp.z);
				box2.Move(tmp.x, tmp.y, tmp.z);
				thetmp = tmp;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
		}

		if (currentkeystate[SDL_SCANCODE_D])
		{
			d = true;
			if (noclip)//gravity
			{
				Vec3d tmp(cameraright.x*movementspeed, cameraright.y*movementspeed, cameraright.z*movementspeed);
				cameralocation = cameralocation - tmp;
				ak47.Move(-tmp.x, -tmp.y, -tmp.z);
				Grunt.Move(-tmp.x, -tmp.y, -tmp.z);
				box2.Move(-tmp.x, -tmp.y, -tmp.z);
				thetmp = tmp * -1;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
			else
			{
				Vec3d tmp(-originalplayerbasis.one.x*movementspeed, 0, -originalplayerbasis.one.z*movementspeed);
				cameralocation = cameralocation - tmp;
				ak47.Move(-tmp.x, -tmp.y, -tmp.z);
				Grunt.Move(-tmp.x, -tmp.y, -tmp.z);
				box2.Move(-tmp.x, -tmp.y, -tmp.z);
				thetmp = tmp * -1;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
		}
		if (currentkeystate[SDL_SCANCODE_S])
		{
			s = true;
			if (noclip)//gravity
			{
				Vec3d tmp(cameralookat.x*movementspeed, cameralookat.y*movementspeed, cameralookat.z*movementspeed);
				cameralocation = cameralocation + tmp;
				ak47.Move(tmp.x, tmp.y, tmp.z);
				Grunt.Move(tmp.x, tmp.y, tmp.z);
				box2.Move(tmp.x, tmp.y, tmp.z);
				thetmp = tmp;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
			else
			{
				Vec3d tmp(originalplayerbasis.three.x*movementspeed, 0, originalplayerbasis.three.z*movementspeed);
				cameralocation = cameralocation + tmp;
				ak47.Move(tmp.x, tmp.y, tmp.z);
				Grunt.Move(tmp.x, tmp.y, tmp.z);
				box2.Move(tmp.x, tmp.y, tmp.z);
				thetmp = tmp;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
		}
		if (currentkeystate[SDL_SCANCODE_W])
		{
			w = true;
			if (noclip)//gravity
			{
				Vec3d tmp(cameralookat.x*movementspeed, cameralookat.y*movementspeed, cameralookat.z*movementspeed);
				cameralocation = cameralocation - tmp;
				ak47.Move(-tmp.x, -tmp.y, -tmp.z);
				Grunt.Move(-tmp.x, -tmp.y, -tmp.z);
				box2.Move(-tmp.x, -tmp.y, -tmp.z);
				thetmp = tmp * -1;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
			else
			{
				Vec3d tmp(originalplayerbasis.three.x*movementspeed, 0, originalplayerbasis.three.z*movementspeed);
				cameralocation = cameralocation - tmp;
				ak47.Move(-tmp.x, -tmp.y, -tmp.z);
				Grunt.Move(-tmp.x, -tmp.y, -tmp.z);
				box2.Move(-tmp.x, -tmp.y, -tmp.z);
				thetmp = tmp * -1;
				for (int i = 0; i < theboxes.size(); i++)
				{
					playermoveback(box2, theboxes[i], thetmp);
				}
			}
		}
		if (currentkeystate[SDL_SCANCODE_LCTRL] && noclip)
		{
			Vec3d tmp(cameraup.x*movementspeed, cameraup.y*movementspeed, cameraup.z*movementspeed);
			cameralocation = cameralocation - tmp;
			ak47.Move(-tmp.x, -tmp.y, -tmp.z);
			Grunt.Move(-tmp.x, -tmp.y, -tmp.z);
			box2.Move(-tmp.x, -tmp.y, -tmp.z);
			thetmp = tmp*-1;
			for (int i = 0; i < theboxes.size(); i++)
			{
				playermoveback(box2, theboxes[i], thetmp);
			}
		}
		if (currentkeystate[SDL_SCANCODE_SPACE] && noclip)
		{
			Vec3d tmp(cameraup.x*movementspeed, cameraup.y*movementspeed, cameraup.z*movementspeed);
			cameralocation = cameralocation + tmp;
			ak47.Move(tmp.x, tmp.y, tmp.z);
			Grunt.Move(tmp.x, tmp.y, tmp.z);
			box2.Move(tmp.x, tmp.y, tmp.z);
			thetmp = tmp;
			for (int i = 0; i < theboxes.size(); i++)
			{
				playermoveback(box2, theboxes[i], thetmp);
			}
		}
		

		SDL_SetRenderDrawColor(grender, 255, 255, 255, 0);
		SDL_RenderClear(grender);

		/*Object Transform*/
		//thetriangle.Rotate(0, 0.00001, 0, tricenter);
		//thesquare.Rotate(0.00001, 0.00001, 0.00001, squarecenter);
		//SpaceShip.Rotate(0, 0.00010, 0, tricenter);
		//ak47.Rotate(0, 0.00010, 0, mountaincenter);
		//MasterChief.Rotate(0, 0.001, 0, MasterChief.Triangles[0].one);
		//EnergySword.Rotate(0, .001, 0, EnergySword.Triangles[0].one);
		//CrashB.Rotate(0, .0001, 0, CrashB.Triangles[0].one);
		

		if (cameratwo)//cameratwo
		{
			CalculateViewPlanes2();
			cameralocation4d.x = cameralocation2.x; cameralocation4d.y = cameralocation2.y; cameralocation4d.z = cameralocation2.z; cameralocation4d.w = 1;
		}
		else if (camerathree)
		{
			CalculateViewPlanes3();
			cameralocation4d.x = cameralocation3.x; cameralocation4d.y = cameralocation3.y; cameralocation4d.z = cameralocation3.z; cameralocation4d.w = 1;
		}
		else
		{
			CalculateViewPlanes();
			cameralocation4d.x = cameralocation.x; cameralocation4d.y = cameralocation.y; cameralocation4d.z = cameralocation.z; cameralocation4d.w = 1;
		}

		/*bullet physics*/
		float bulletspeed = .1f;
		float gravityspeed = .3f;
		int thesize = bullets.size();
		int ncounter = 0;

		for (int i = 0; i < thesize; i++)
		{
			Vec3d bulletvector;
			if (bullets[ncounter].lifespan == 0)
			{
				bulletvector = (cameraright*.06 * -1);
				bullets[ncounter].mesh->Triangles = bullet.Triangles;
				bullets[ncounter].mesh->RodriguesRotation(dummy, aktheta);
				bullets[ncounter].mesh->RodriguesRotation(axisright, akphi);
				bullets[ncounter].mesh->Move(ak47.Triangles[1279].one.x, ak47.Triangles[1279].one.y, ak47.Triangles[1279].one.z);
				bullets[ncounter].mesh->Move(bulletvector.x, bulletvector.y, bulletvector.z);
				
				Vec3d zz(ak47.Triangles[1279].one.x + bulletvector.x, ak47.Triangles[1279].one.y + bulletvector.y, ak47.Triangles[1279].one.z + bulletvector.z);
				bullets[ncounter].originalmove = zz;
			}
			if (bullets[ncounter].lifespan == 1)
			{
				bulletvector = (cameraright*bulletspeed * -1);
				bullets[ncounter].mesh->Triangles = bullet.Triangles;
				bullets[ncounter].mesh->RodriguesRotation(dummy, aktheta);
				bullets[ncounter].mesh->RodriguesRotation(axisright, akphi);
				bullets[ncounter].mesh->Move(ak47.Triangles[1279].one.x, ak47.Triangles[1279].one.y, ak47.Triangles[1279].one.z);
				bullets[ncounter].mesh->Move(bulletvector.x, bulletvector.y, bulletvector.z);

				Vec3d zz(ak47.Triangles[1279].one.x + bulletvector.x, ak47.Triangles[1279].one.y + bulletvector.y, ak47.Triangles[1279].one.z + bulletvector.z);
				bullets[ncounter].originalmove = zz;
			}
			if (bullets[ncounter].lifespan == 2)
			{
				bulletvector = (cameraright*.15 * -1);
				bullets[ncounter].mesh->Triangles = bullet.Triangles;
				bullets[ncounter].mesh->RodriguesRotation(dummy, aktheta);
				bullets[ncounter].mesh->RodriguesRotation(axisright, akphi);
				bullets[ncounter].mesh->Move(ak47.Triangles[1279].one.x, ak47.Triangles[1279].one.y, ak47.Triangles[1279].one.z);
				bullets[ncounter].mesh->Move(bulletvector.x, bulletvector.y+.05, bulletvector.z);
			}
			bullets[ncounter].lifespan++;
			if (bullets[ncounter].lifespan > 3)
			{
				delete bullets[ncounter].mesh;
				bullets.erase(bullets.begin() + ncounter);
				ncounter--;
			}
			ncounter++;
		}
		if (shooting && shootcounter >= 3)//5
		{
			if (currentammo > 0)
				currentammo--;
				
			/*spawn a bullet*/
			bulletstruct tmp;
			Mesh* newbullet = new Mesh;
			for (int i = 0; i < bullet.Triangles.size(); i++)
			{
				newbullet->AddTriangle(bullet.Triangles[i]);
			}
			newbullet->RodriguesRotation(dummy, aktheta);
			newbullet->RodriguesRotation(axisright, akphi);
			newbullet->Move(ak47.Triangles[1279].one.x, ak47.Triangles[1279].one.y, ak47.Triangles[1279].one.z);
			tmp.mesh = newbullet;
			tmp.velocity = cameraright * -1;
			Vec3d zz(ak47.Triangles[1279].one.x, ak47.Triangles[1279].one.y, ak47.Triangles[1279].one.z);
			tmp.originalmove = zz;
			tmp.theta = aktheta;
			tmp.phi = akphi;
			bullets.push_back(tmp);

			/*Spawn a ray and check if it hit any targets*/
			Ray ray1;
			Vec4d rayposition(ak47.Triangles[555].one.x, ak47.Triangles[555].one.y, ak47.Triangles[555].one.z,1);
			Vec4d lookat4(-cameralookat.x, -cameralookat.y, -cameralookat.z, 1);
			rayposition.x = cameralocation.x;
			rayposition.y = cameralocation.y;// + .1;
			rayposition.z = cameralocation.z;// +.2;
			rayposition = rayposition + (lookat4 * .8);
			Vec4d raydirection(-cameralookat.x, -cameralookat.y, -cameralookat.z, 1);
			raydirection = raydirection.normalize();
			ray1.setray(rayposition, raydirection);
			for(int i = 0;i<targetmeshes.size();i++)
			{
				for(int t = 0;t<targetmeshes[i].Triangles.size();t++)
				{
					if(checkcollision(ray1,targetmeshes[i].Triangles[t]))
					{
						targetmeshes[i].Triangles[t].one.r = 255;
						targetmeshes[i].Triangles[t].one.g = 0;
						targetmeshes[i].Triangles[t].one.b = 0;
					}
				}
			}
			shootcounter = 0;
		}

		if (shooting)
		{
			if (shootcounter % 3 < 3-1)
			{
				pitch-=.5;
			}
			else
			{
				pitch+=.5;
			}
		}
		shootcounter++;
		if (currentammo == 0)
		{
			aiming = false;
			if (Mix_PlayingMusic() && !reloading)
			{
				Mix_HaltMusic();
				Mix_PlayChannel(0, akshootchunk, 0);
			}
			if (!Mix_Playing(1) && !reloading)
			{
				Mix_PlayChannel(1, akreload, 0);
				reloading = true;
				shooting = false;
			}
			if (!Mix_Playing(1))
			{
				currentammo = maxammo;
				reloading = false;
			}
		}
		if (pressreload)
		{
			aiming = false;
			if (Mix_PlayingMusic() && !reloading)
			{
				Mix_HaltMusic();
				Mix_PlayChannel(0, akshootchunk, 0);
			}
			if (!Mix_Playing(1) && !reloading)
			{
				Mix_PlayChannel(1, akreload, 0);
				reloading = true;
				shooting = false;
			}
			if (!Mix_Playing(1))
			{
				currentammo = maxammo;
				reloading = false;
				pressreload = false;
			}
		}

		//CrashB.RodriguesRotation(axis, .1);
		/*clipping slowest thing by far*/
		///*BSP RENDERING*/
		Vec4d e(cameralocation.x, cameralocation.y, cameralocation.z, 1);
		//bsp1.DrawStart(grender, e,slow);

		//Mesh tmptriangle = ak47;
		//tmptriangle.Draw(0, 155, 100);
		
		/*SORTED TRIANGLE RENDERING*/
		RenderedSortedTriangle();
	
		
		/*PER OBJECT RENDERING*/
		//Mesh tmptriangle = thetriangle;
		//tmptriangle.Print(255, 0, 0);

		//Mesh tmpsquare = thesquare;
		//tmpsquare.Print(255, 0, 0);

		//Mesh tmpspaceship = SpaceShip;
		//tmpspaceship.Draw(0, 255, 0);

		//Mesh tmpcheif = MasterChief;
		//tmpcheif.Print(0, 0, 255);
		
		//Mesh tmpenergysword = EnergySword;
		//tmpenergysword.Print(0, 0, 255);

		//Mesh tmpcrash = CrashB;
		//tmpcrash.Draw(55, 55, 155);

		//Mesh tmpScarab = Scarab;
		//tmpScarab.Draw(0, 165, 100);

		/*GUI RENDERING*/
		if (camerathree)
		{

		}
		else if (cameratwo)
		{

		}
		else
		{
			if (!aiming)
			{
				SDL_Rect middle = { (screenwidth / 2) - 20,(screenheight / 2) - 20, 40,40 };
				SDL_RenderCopy(grender, crosshair, 0, &middle);
			}
		}
		
		
		if (shooting)
		{
			//put in on aks triangleposition
			Vec4d position;
			position = ak47.Triangles[555].one * -1;
		}
		if (reloading)
		{
			if (!reloadanimationonce)
			{
				ak47.Triangles = akoriginal;
				ak47.RodriguesRotation(axisz, 1.57);
				ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
				ak47.Move(.2, .2, .5);
				reloadanimationonce = true;
				justreloaded = true;
			}
		}
		else
		{
			if (reloadanimationonce)
			{
				if (justreleased)
				{
					reloadanimationonce = false;
				}
				else
				{
					ak47.Triangles = akoriginal;
					//ak47.RodriguesRotation(axisz, 1.57);
					ak47.Move(cameralocation.x, cameralocation.y, cameralocation.z);
					ak47.Move(.2, .2, .5);
					reloadanimationonce = false;
					justreloaded = true;
				}
			}
		}

		SDL_Color black = { 0,0,0 };
		SDL_Color white = { 255,255,255 };
		SDL_Color yellow = { 255,255,0 };
		ammocount = loadtext(to_string(currentammo), white);


		SDL_Rect currentammobox = { screenwidth - 115,screenheight - 50,to_string(currentammo).length()*15,30 };
		SDL_RenderCopy(grender, ammocount, 0, &currentammobox);
		SDL_DestroyTexture(ammocount);
		
		ammomax = loadtext("/", black);
		SDL_Rect slashbox = { screenwidth - 115 + to_string(currentammo).length() * 15,screenheight - 50,10,30 };
		SDL_RenderCopy(grender, ammomax, 0, &slashbox);
		SDL_DestroyTexture(ammocount);

		ammomax = loadtext(to_string(maxammo), black);
		SDL_Rect maxammobox = { screenwidth - 115 + to_string(currentammo).length() * 15 + 10,screenheight - 50,to_string(maxammo).length() * 15,30 };
		SDL_RenderCopy(grender, ammomax, 0, &maxammobox);
		SDL_DestroyTexture(ammocount);

		/*Render menu */
		
		if (menuon)
		{
			SDL_Rect backgroundbox = { screenwidth / 2-200, screenheight / 2 - 200,400,400 };
			SDL_SetRenderDrawColor(grender, 155, 155, 155, 150);
			SDL_SetRenderDrawBlendMode(grender, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(grender, &backgroundbox);
			SDL_SetRenderDrawBlendMode(grender, SDL_BLENDMODE_NONE);
	
			if (menu1)
			{
				SDL_Rect titlebox = { screenwidth / 2 - 75, screenheight / 2 - 200,150,50 };
				menuwords = loadtext("MENU", black);
				SDL_RenderCopy(grender, menuwords, 0, &titlebox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect instructionsbox = { screenwidth / 2 - 150, screenheight / 2 - 50,300,25 };
				menuwords = loadtext("INSTRUCTIONS", yellow);
				SDL_RenderCopy(grender, menuwords, 0, &instructionsbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect quitbox = { screenwidth / 2 - 50, screenheight / 2 + 100,100,25 };
				menuwords = loadtext("QUIT", black);
				SDL_RenderCopy(grender, menuwords, 0, &quitbox);
				SDL_DestroyTexture(menuwords);

			}
			else if (menu2)
			{
				SDL_Rect titlebox = { screenwidth / 2 - 75, screenheight / 2 - 200,150,50 };
				menuwords = loadtext("MENU", black);
				SDL_RenderCopy(grender, menuwords, 0, &titlebox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect instructionsbox = { screenwidth / 2 - 150, screenheight / 2 - 50,300,25 };
				menuwords = loadtext("INSTRUCTIONS", black);
				SDL_RenderCopy(grender, menuwords, 0, &instructionsbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect quitbox = { screenwidth / 2 - 50, screenheight / 2 + 100,100,25 };
				menuwords = loadtext("QUIT", yellow);
				SDL_RenderCopy(grender, menuwords, 0, &quitbox);
				SDL_DestroyTexture(menuwords);

			}
			else if (menu3)
			{
				int ls = 10;
				SDL_Rect titlebox = { screenwidth / 2 - 150, screenheight / 2 - 200,300,50 };
				menuwords = loadtext("INSTRUCTIONS", black);
				SDL_RenderCopy(grender, menuwords, 0, &titlebox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect onebox = { screenwidth / 2 - 150, screenheight / 2 - 100,ls *27,25 };
				menuwords = loadtext("1 - toggle spectator camera", black);
				SDL_RenderCopy(grender, menuwords, 0, &onebox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect twobox = { screenwidth / 2 - 150, screenheight / 2 - 75,ls * 28,25 };
				menuwords = loadtext("2 - toggle 3rd person camera", black);
				SDL_RenderCopy(grender, menuwords, 0, &twobox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect gbox = { screenwidth / 2 - 150, screenheight / 2 - 50,ls * 23,25 };
				menuwords = loadtext("g - reset hit detection", black);
				SDL_RenderCopy(grender, menuwords, 0, &gbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect tildebox = { screenwidth / 2 - 150, screenheight / 2 - 25,ls * 18,25 };
				menuwords = loadtext("` - toggle no clip", black);
				SDL_RenderCopy(grender, menuwords, 0, &tildebox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect minusbox = { screenwidth / 2 - 150, screenheight / 2 - 0,ls * 19,25 };
				menuwords = loadtext("- - show wireframes", black);
				SDL_RenderCopy(grender, menuwords, 0, &minusbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect pbox = { screenwidth / 2 - 150, screenheight / 2 + 25,ls * 22,25 };
				menuwords = loadtext("p - toggle pointlights", black);
				SDL_RenderCopy(grender, menuwords, 0, &pbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect ammobox = { screenwidth / 2 - 150, screenheight / 2 + 50,ls * 22,25 };
				menuwords = loadtext("8/9 - sub/add max ammo", black);
				SDL_RenderCopy(grender, menuwords, 0, &ammobox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect bbox = { screenwidth / 2 - 150, screenheight / 2 + 75,ls * 19,25 };
				menuwords = loadtext("b - delay rendering", black);
				SDL_RenderCopy(grender, menuwords, 0, &bbox);
				SDL_DestroyTexture(menuwords);

				SDL_Rect mbox = { screenwidth / 2 - 150, screenheight / 2 + 100,ls * 24,25 };
				menuwords = loadtext("m - move and show cursor", black);
				SDL_RenderCopy(grender, menuwords, 0, &mbox);
				SDL_DestroyTexture(menuwords);
			}
		}
		mousedelay++;

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		fps = 1 / (duration.count() / 1000000.0f);
		int fpsint = fps;
		string fpsstring = "FPS: ";
		fpsstring.append(to_string(fpsint));
		const char* lol = fpsstring.c_str();
		SDL_SetWindowTitle(gwindow, lol);

		if (duration.count() < 16600.f)
		{
			SDL_Delay((16600.f - duration.count()) / 1000.f);
		}
		SDL_RenderPresent(grender);
	}


	return 0;
}

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Could not initialize SDL";
		success = false;
	}
	else
	{
		gwindow = SDL_CreateWindow("Computer Graphics Practice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenwidth, screenheight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (gwindow == 0)
		{
			cout << "could not create window";
			success = false;
		}
		else
		{
			gsurface = SDL_GetWindowSurface(gwindow);
			if (gsurface == 0)
			{
				cout << "could not upload surface";
				success = false;
			}
			else
			{
				int flags = IMG_INIT_PNG;
				if ((IMG_Init(flags) & flags) != flags)
				{
					cout << "failed to initialize png";
					success = false;
				}
				if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
				{
					cout << "failed to scale quality";
				}
				grender = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED);
				SDL_SetRenderDrawColor(grender, 255, 255, 255, 255);

			}
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		}
	}
	return success;
}
bool loadmedia()
{
	bool success = true;
	SDL_Color colorblack = { 0,0,0 };
	gfont = TTF_OpenFont("times-new-roman.ttf", 28);
	ammocount = loadtext(to_string(currentammo), colorblack);
	string maxammo2 = "/ ";
	maxammo2.append(to_string(maxammo));
	ammomax = loadtext(maxammo2, colorblack);

	crosshair = loadimage("crosshair.png");
	akshoot = Mix_LoadMUS("akloop.wav");
	akreload = Mix_LoadWAV("ak47reload.wav");
	akshootchunk = Mix_LoadWAV("aklastbullet.wav");

	return success;
}
SDL_Texture* loadimage(string a)
{
	SDL_Surface* tempsurf;
	tempsurf = IMG_Load(a.c_str());
	if (tempsurf == 0)
	{
		cout << "could not load image";
	}
	SDL_Texture* temptext;
	temptext = SDL_CreateTextureFromSurface(grender, tempsurf);

	SDL_FreeSurface(tempsurf);
	return temptext;
}
SDL_Texture* loadtext(string s, SDL_Color textcolor)
{
	SDL_Surface* tempsurface = TTF_RenderText_Blended_Wrapped(gfont, s.c_str(), textcolor, screenwidth);
	SDL_Texture* returntexture;
	returntexture = SDL_CreateTextureFromSurface(grender, tempsurface);
	//cout << tempsurface->w << " " << tempsurface->h;
	//get width and height
	SDL_FreeSurface(tempsurface);
	return returntexture;
}

/*this algorithm is close to the trivial line drawing one but it uses some algebra to make it so we dont have to use multiplication or division, we can write
the next y in terms of the initial one plus a constant and increment the other by one*/
void LineDDA()
{
	SDL_SetRenderDrawColor(grender, 255, 0, 0, 0);
	
	float xa, ya, xb, yb;
	float dx, dy;
	float deno;
	float xincrement, yincrement;
	float currentx, currenty;
	int i = 0;

	xa = 100; ya = 200;
	xb = 105; yb = 500;

	dx = xb - xa;
	dy = yb - ya;

	if (abs(dy) < abs(dx))
	{
		deno = abs(dx);
	}
	else
	{
		deno = abs(dy);
	}

	xincrement = (dx / deno);
	yincrement = (dy / deno);

	currentx = xa;
	currenty = ya;

	for (i; i < deno; i++)
	{
		SDL_RenderDrawPoint(grender, currentx, currenty);
		currentx = currentx + xincrement;
		currenty = currenty + yincrement;
	}


}

/*this algorithm makes it so you only have to use integers. You add one dimension by 1 and see if the other one should increment or not based on the difference between
the y on the line and the yi and yi+1. The closer the actual line is so yi we dont add, if actual line is closer to yi+1 we had 1 to y. Uses algebra to do this, and make 
the calculations simpler especially for the deicision parameter not using m in calculations.*/
void LineBresenhams(float xa, float ya, float xb, float yb)
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
		for (i; i <= dx*signx; i++)
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
		p = 2*dx*signx - dy * signy;
		for (i; i <= dy*signy; i++)
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

/*this algorithm is kind of the same as his line one, you start at 90 degrees and increment x and see if y needs to be lowered or stay the same. This is determined by
x^2 + y^2 - R^2 = 0 and  yi+1 is the midpoint yi - 1/2. if its outside the circle lower it but if its already inside dont do anything to it. Then we use symmetry to find
the other 7 sectors of the circle.*/
void CircleBresenhamsMidPoint()
{
	SDL_SetRenderDrawColor(grender, 0, 0, 255, 0);
	
	int xc, yc;
	int currentx, currenty;
	int R;
	int p;

	xc = 200; yc = 200;
	R = 100;
	currentx = 0;
	currenty = R;
	p = 1 - R;

	for (int i = 0; currentx <= currenty; i++)
	{
		SDL_RenderDrawPoint(grender, currentx + xc, currenty + yc);
		SDL_RenderDrawPoint(grender, currentx + xc, currenty + yc);
		SDL_RenderDrawPoint(grender, -currentx + xc, currenty + yc);
		SDL_RenderDrawPoint(grender, -currentx + xc, -currenty + yc);
		SDL_RenderDrawPoint(grender, currentx + xc, -currenty + yc);
		SDL_RenderDrawPoint(grender, currenty + xc, currentx + yc);
		SDL_RenderDrawPoint(grender, -currenty + xc, currentx + yc);
		SDL_RenderDrawPoint(grender, -currenty + xc, -currentx + yc);
		SDL_RenderDrawPoint(grender, currenty + xc, -currentx + yc);
		if (p < 0)
		{
			currentx = currentx + 1;
			currenty = currenty;
			p = p + 2 * currentx + 3;
		}
		else
		{
			currentx = currentx + 1;
			currenty = currenty - 1;
			p = p + 2 * currentx - 2 * currenty + 5;
		}
	}


}
void CircleTrivial()
{
	SDL_SetRenderDrawColor(grender, 0, 0, 255, 0);
	int xc, yc;
	double R;
	int x, y;
	xc = 200; yc = 200;
	R = 5;
	double increment = 50 / R;
	bool leave = true;
	while (!leave)
	{
		double z = 45 / increment;
		if (z - static_cast<int>(z) != 0)
		{
			increment -= .1;
		}
		else
		{
			increment = z;
			leave = true;
		}
	}
	for (double i = 0; i < 45; i+=increment)
	{
		x = R * cos((i * 180) / 3.14);
		y = R * sin((i * 180) / 3.14);
		SDL_RenderDrawPoint(grender, x + xc, y + yc);
		SDL_RenderDrawPoint(grender, -x + xc, y + yc);
		SDL_RenderDrawPoint(grender, -x + xc, -y + yc);
		SDL_RenderDrawPoint(grender, x + xc, -y + yc);
		SDL_RenderDrawPoint(grender, y + xc, x + yc);
		SDL_RenderDrawPoint(grender, -y + xc, x + yc); 
		SDL_RenderDrawPoint(grender, -y + xc, -x + yc); 
		SDL_RenderDrawPoint(grender, y + xc, -x + yc);
	}
}

/*algorithm is just like the circle one but you use the ellipse equation inside, and there can only be 4 symmetry points, and you have so split each corner into 2
regions because in one the slope is less than 1 and the other its greater. It works just by incrementing and seeing if we need to increment the other or not.*/
void EllipseBresenhamsMidpoint()
{
	SDL_SetRenderDrawColor(grender, 0, 255, 0, 0);
	
	int xc, yc, x1, y1, x2, y2, Rx, Ry, p1, p2;

	xc = 500;
	yc = 300;
	Rx = 300;
	Ry = 200;
	int Rx2 = Rx * Rx;
	int Ry2 = Ry * Ry;

	x1 = 0;
	y1 = Ry;
	p1 = Ry2 + (1/4)*Rx2 - Ry * Rx2;

	SDL_RenderDrawPoint(grender, x1 + xc, y1 + yc);
	for (int i = 0; Ry2*x1 <= Rx2*y1; i++)
	{
		SDL_RenderDrawPoint(grender, x1 + xc, y1 + yc);
		SDL_RenderDrawPoint(grender, -x1 + xc, y1 + yc);
		SDL_RenderDrawPoint(grender, -x1 + xc, -y1 + yc);
		SDL_RenderDrawPoint(grender, x1 + xc, -y1 + yc);
		if (p1 <= 0)
		{
			x1 += 1;
			y1 = y1;
			p1 = p1 + 2 * Ry2*x1 + Ry2;								//p1 + (2 * x1 + 3)*Ry2;
		}
		else
		{
			x1 += 1;
			y1 -= 1;
			p1 = p1 + 2 * Ry2*x1 - 2 * Rx2*y1 + Ry2;				//p1 + ((2 * x1 + 3)*Ry2) + (((-2 * y1) + 2)*Rx2);
		}
	}

	x2 = Rx;
	y2 = 0;
	p2 = Rx2 - Rx * Ry2;

	SDL_RenderDrawPoint(grender, x2 + xc, y2 + yc);
	for (int i = 0; Rx2*y2 <= Ry2*x2; i++)
	{
		SDL_RenderDrawPoint(grender, x2 + xc, y2 + yc);
		SDL_RenderDrawPoint(grender, -x2 + xc, y2 + yc);
		SDL_RenderDrawPoint(grender, -x2 + xc, -y2 + yc);
		SDL_RenderDrawPoint(grender, x2 + xc, -y2 + yc);
		if (p2 <= 0)
		{
			x2 = x2;
			y2 = y2 + 1;
			p2 = p2 + 2 * Rx2*y2 + Rx2;								//p2 + (2 * y2 + 3)*Rx2;
		}
		else
		{
			x2 = x2 - 1;
			y2 = y2 + 1;
			p2 = p2 + 2 * Rx2*y2 - 2 * Ry2*x2 + Rx2;				//p2 - (2 * x2 - 2) * Ry2 + (2 * y2 + 3) * Rx2;
		}
	}
}

/*very simple algorithm, only hard part is determining increment. For increment we want to increment by 1 pixel so we have a right angle triangle with radius and height
1, and we just find the angle of this triangle which would give us an incremental angle to increase arc by 1 (it uses small angle theorom to get rid of arctan). The rest
is just easy its finding the points on a circle with angle x, incrementing x and doing again until we have our arcs.*/
void ClockWiseArc()
{
	SDL_SetRenderDrawColor(grender, 255, 0, 255, 0);

	int xc, yc;
	float x, y = 0;
	int r;
	float theta1 = 0;
	float theta2 = 0;
	float theta = 0;
	float thetaincrement = 0;

	xc = 500;
	yc = 300;
	theta1 = M_PI;
	theta2 = 2*M_PI;
	r = 100;

	thetaincrement = 1.0 / r;
	if (theta1 < theta2)
	{
		theta2 = theta2 - 2 * M_PI;
	}
	theta = theta1;
	for (int i = 0; theta >= theta2; i++)
	{
		x = r * cos(theta);
		y = r * sin(theta);
		theta = theta - thetaincrement;
		SDL_RenderDrawPoint(grender, x + xc, -y + yc);
	}

}
void LoadOBJ(Mesh& meshname, string filename)
{
	cout << filename << " : ";
	vector<Vec4d> vertices;
	float x, y, z;
	int one, two, three;
	Vec4d tmp;
	Triangle3D tmptri;
	fstream file;
	string holder;
	file.open(filename, ios::in);
	bool leave = false;
	while (!file.eof())
	{
		if (file.peek() == SDLK_v)
		{
			getline(file, holder, 'v');
			if (file.peek() == ' ')
			{
				getline(file, holder, ' ');
				getline(file, holder, ' ');
				getline(file, holder, ' ');
				x = stof(holder);
				getline(file, holder, ' ');
				y = stof(holder);
				getline(file, holder);
				z = stof(holder);

				tmp.x = x; tmp.y = y; tmp.z = z; tmp.w = 1;
				vertices.push_back(tmp);
			}
			else
			{
				getline(file, holder);
				//cout << holder << endl;
			}
		}
		else
		{	
			getline(file, holder);
		}
	}
	//cout << vertices.size()<<" "<<endl;
	file.close();
	file.open(filename, ios::in);
	leave = false;

	while (!file.eof())
	{
		if (file.peek() == SDLK_f)
		{
			getline(file, holder, ' ');
			getline(file, holder, '/');
			one = stoi(holder);
			getline(file, holder, ' ');
			getline(file, holder, '/');
			two = stoi(holder);
			getline(file, holder, ' ');
			getline(file, holder, '/');
			three = stoi(holder);
			getline(file, holder);
			//cout << "vertice size: " << vertices.size() << " ";
			//cout << "one: " << one << " two: " << two << " three: " << three << endl;
			tmptri.one = vertices[one - 1];
			tmptri.two = vertices[two - 1];
			tmptri.three = vertices[three - 1];
			meshname.AddTriangle(tmptri);
		}
		else
		{
			getline(file, holder);
		}
	}
	file.close();
	cout << meshname.Triangles.size() << endl;
	//meshname.Scale(.01, .01, .01);
}
void LoadSpaceShip(Mesh& meshname, string filename)
{
	cout << filename << " : ";
	vector<Vec4d> vertices;
	float x, y, z;
	int one, two, three;
	Vec4d tmp;
	Triangle3D tmptri;
	fstream file;
	string holder;
	file.open(filename, ios::in);

	bool leave = false;
	while (!leave)
	{
		getline(file, holder, ' ');
		//v
		getline(file, holder, ' ');
		x = stof(holder);
		getline(file, holder, ' ');
		y = stof(holder);
		getline(file, holder);
		z = stof(holder);
		tmp.x = x; tmp.y = y; tmp.z = z; tmp.w = 1;
		vertices.push_back(tmp);
		if (file.peek() == SDLK_f)
		{
			leave = true;
		}
	}
	while (!file.eof())
	{
		getline(file, holder, ' ');
		//f
		getline(file, holder, ' ');
		one = stoi(holder);
		getline(file, holder, ' ');
		two = stoi(holder);
		getline(file, holder);
		three = stoi(holder);
		//cout << "vertice size: " << vertices.size() << " ";
			//cout << "one: " << one << " two: " << two << " three: " << three << endl;
		tmptri.one = vertices[one-1];
		tmptri.two = vertices[two-1];
		tmptri.three = vertices[three-1];
		meshname.AddTriangle(tmptri);
	}
	file.close();
	cout << meshname.Triangles.size() << endl;
	//meshname.Scale(.1, .1, .1);
}

Vec3d crossproduct(Vec3d one, Vec3d two)
{
	Vec3d p;
	p.x = one.y * two.z - one.z * two.y;
	p.y = one.z * two.x - one.x * two.z;
	p.z = one.x * two.y - one.y * two.x;

	return p;
}

void updatecamera()//**I think im actuallyy changing values with transformations so the z's get messed up maybe make sure they are passed in as constants
{
	Vec3d worldupdirection(0, -1, 0);

	if (pitch > 179.0f)
	{
		pitch = 179.0f;
	}

	if (pitch < 1.0f)
	{
		pitch = 1.0f;
	}

	cameralookat.x = sin(toradians(pitch)) * cos(toradians(yaw));
	cameralookat.y = cos(toradians(pitch));
	cameralookat.z = sin(toradians(pitch)) * sin(toradians(yaw));
	cameralookat = normalize(cameralookat);



	cameraright = normalize(crossproduct(cameralookat, worldupdirection));
	cameraup = normalize(crossproduct(cameraright, cameralookat));	
	
}

void updatecamera2()//**I think im actuallyy changing values with transformations so the z's get messed up maybe make sure they are passed in as constants
{
	Vec3d worldupdirection(0, -1, 0);

	if (pitch2 > 179.0f)
	{
		pitch2 = 179.0f;
	}

	if (pitch2 < 1.0f)
	{
		pitch2 = 1.0f;
	}

	cameralookat2.x = sin(toradians(pitch2)) * cos(toradians(yaw2));
	cameralookat2.y = cos(toradians(pitch2));
	cameralookat2.z = sin(toradians(pitch2)) * sin(toradians(yaw2));
	cameralookat2 = normalize(cameralookat2);



	cameraright2 = normalize(crossproduct(cameralookat2, worldupdirection));
	cameraup2 = normalize(crossproduct(cameraright2, cameralookat2));

}
void updatecamera3(bool justx)
{
	Vec3d worldupdirection(0, -1, 0);
	if (!justx)
	{
		float threepitch = pitch + 20;
		if (threepitch > 179.0f)
		{
			threepitch = 179.0f;
		}

		if (threepitch < 1.0f)
		{
			threepitch = 1.0f;
		}
		pitch3 = threepitch;

		cameralookat3.x = sin(toradians(threepitch)) * cos(toradians(yaw));
		cameralookat3.y = cos(toradians(threepitch));
		cameralookat3.z = sin(toradians(threepitch)) * sin(toradians(yaw));
		cameralookat3 = normalize(cameralookat3);
	}
	else
	{
		cout << pitch3 <<"  "<<yaw<< endl;
		cameralookat3.x = sin(toradians(pitch3)) * cos(toradians(yaw));
		cameralookat3.y = cos(toradians(pitch3));
		cameralookat3.z = sin(toradians(pitch3)) * sin(toradians(yaw));
		cameralookat3 = normalize(cameralookat3);
	}

	cameraright3 = normalize(crossproduct(cameralookat3, worldupdirection));
	cameraup3 = normalize(crossproduct(cameraright3, cameralookat3));
}
float toradians(float degrees)
{
	return (degrees*3.14) / 180.0;
}
Vec3d normalize(Vec3d one)
{

	float length = sqrt(one.x*one.x + one.y*one.y + one.z*one.z);
	Vec3d thereturn = one;
	
	thereturn.x /= length;
	thereturn.y /= length;
	thereturn.z /= length;
	
	return thereturn;
}

void Jump()
{
	Vec3d thetmp(0, 0, 0);

	double value = -pow((float)jumpcounter / 55, 2) + .2;//3*log(jumpcounter+1) //(1/sqrt(jumpcounter+1))*10 //55 , .2
	if (value < 0)
	{
		goingdown = true;
	}
	/*if (jumpcounter < 0)
	{
		goingdown = false;
		isjumping = false;
		jumpcounter = 0;
	}*/
	if (!goingdown && isjumping)
	{
		Vec3d worldupdirection(0, -1 * value, 0);
		Vec3d move = worldupdirection - gravity;
		cameralocation = cameralocation + move;
		ak47.Move(move.x, move.y, move.z);
		Grunt.Move(move.x, move.y, move.z);
		box2.Move(move.x, move.y, move.z);
		thetmp = move;
		for (int i = 0; i < theboxes.size(); i++)
		{
			playermoveback(box2, theboxes[i], move);
		}
		jumpcounter++;
	}
	else if (goingdown)
	{
		Vec3d worldupdirection(0, 1 * value, 0);
		Vec3d move = worldupdirection - gravity;
		cameralocation = cameralocation + move;
		ak47.Move(move.x, move.y, move.z);
		Grunt.Move(move.x, move.y, move.z);
		box2.Move(move.x, move.y, move.z);
		thetmp = move;
		for (int i = 0; i < theboxes.size(); i++)
		{
			playermoveback(box2, theboxes[i], move);
		}
		jumpcounter--;
	}
	if (jumpcounter < 0)
	{
		jumpcounter = 0;
	}
	if (feetonground && jumpcounter > 1)
	{
		goingdown = false;
		isjumping = false;
		jumpcounter = 0;
	}
	if (feetonground && goingdown)
	{
		goingdown = false;
		isjumping = false;
		jumpcounter = 0;
	}

}

void RenderedSortedTriangle()
{
	allstatictriangles.clear();

	/*for (Triangle3D x : thetriangle.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : SpaceShip.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : CrashB.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	for (Triangle3D x : ak47.Triangles)
	{
		allstatictriangles.push_back(x);
	}
	/*for (Triangle3D x : thesquare.Triangles)
	{
		//allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : SpaceShip.Triangles)
	{
		//allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : TeaPot.Triangles)
	{
		//allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : Mountains.Triangles)
	{
		//allstatictriangles.push_back(x);
	}*/
	for (Triangle3D x : theboxestriangles)
	{
		allstatictriangles.push_back(x);
	}
	/*for (Triangle3D x : BattleCreek.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : MasterChief.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : specialforces.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : halogun.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	/*for (Triangle3D x : Kirby.Triangles)
	{
		//allstatictriangles.push_back(x);
	}*/
	for (Triangle3D x : Grunt.Triangles)
	{
		allstatictriangles.push_back(x);
	}

	for (int i = 0; i < targetmeshes.size(); i++)
	{
		for (Triangle3D x : targetmeshes[i].Triangles)
		{
			allstatictriangles.push_back(x);
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		for (Triangle3D x : bullets[i].mesh->Triangles)
		{
			allstatictriangles.push_back(x);
		}
	}

	//render floor first
	for (int i = 0; i < boxtriangles.size(); i++)
	{
		boxtriangles[i].DrawTriangle(1, 1, 1);
	}

	/*calculate colors before clipping*/
	vector<Triangle3D> clippedtriangles;
	vector<Triangle3D> pointclippedtriangles;
	//cout << allstatictriangles[0].one.g << "  ";
	for (int i = 0; i < allstatictriangles.size(); i++)
	{
		Vec4d color(0, 0, 0, 1);
		Vec4d copycolor;
		color.x = (allstatictriangles[i].one.r / 255.f); color.y = (allstatictriangles[i].one.g / 255.f); color.z = (allstatictriangles[i].one.b / 255.f); color.w = 1;
		copycolor = color;
		allstatictriangles[i].Lighting(color, allstatictriangles[i],true);
		allstatictriangles[i].r = color.x; allstatictriangles[i].g = color.y; allstatictriangles[i].b = color.z;
	
		/*calculate lighting without point, then see if point is 0 if it is your done else clip with new colors*/
		vector<Triangle3D> tmptris;
		bool dontpush = false;
		for (int p = 0; p < thepointlights.size(); p++)
		{
			tmptris.clear();
			Vec4d pointnumber(0, 0, 0, 1);
			float pointpercent = allstatictriangles[i].CalculatePointLightsSinglePercent(copycolor, allstatictriangles[i], p);
			if (pointpercent < .1 || !pointlighton)
			{
				//not in point light your done
				//pointclippedtriangles.push_back(allstatictriangles[i]);
			}
			else
			{
				vector<Vec4d> pointnormals;
				vector<Vec4d> pointpoints;
				
				pointnumber = allstatictriangles[i].CalculatePointLightsSingle(copycolor, allstatictriangles[i], p);
				pointnumber = pointnumber * 255.f;
				pointnumber.clamp(255);
				
				bool cutoff = false;
				if (thepointlights[p].left)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2(point1.x, point1.y, point1.z+1, 1);
					Vec4d point3(point1.x, point1.y + 1, point1.z, 1);
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?

					pointnormals.push_back(normal);
					pointpoints.push_back(point2);
					cutoff = true;
				}
				if (thepointlights[p].right)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2; point2 = point1; point2.z += 1;
					Vec4d point3; point3 = point1; point3.y += 1;
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?
					normal = normal * -1;

					pointnormals.push_back(normal);
					pointpoints.push_back(point1);
					cutoff = true;
				}
				if (thepointlights[p].forward)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2(point1.x + 1, point1.y, point1.z, 1);
					Vec4d point3(point1.x, point1.y + 1, point1.z, 1);
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?
					
					pointnormals.push_back(normal);
					pointpoints.push_back(point2);
					cutoff = true;
				}
				if (thepointlights[p].backward)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2; point2 = point1; point2.x += 1;
					Vec4d point3; point3 = point1; point3.y += 1;
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?
					normal = normal * -1;

					pointnormals.push_back(normal);
					pointpoints.push_back(point1);
					cutoff = true;
				}
				if (thepointlights[p].top)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2; point2 = point1; point2.x += 1; 
					Vec4d point3; point3 = point1; point3.z += 1;
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?

					pointnormals.push_back(normal);
					pointpoints.push_back(point1);
					cutoff = true;
				}
				if (thepointlights[p].bottom)
				{
					Vec4d point1(-thepointlights[p].lightposition.x, -thepointlights[p].lightposition.y, -thepointlights[p].lightposition.z, 1);
					Vec4d point2; point2 = point1; point2.x += 1;
					Vec4d point3; point3 = point1; point3.z += 1;
					Vec4d p1; p1 = point2 - point1;
					Vec4d p2; p2 = point3 - point1;
					Vec4d normal = crossproduct(p1, p2); //normalize?
					normal = normal * -1;

					pointnormals.push_back(normal);
					pointpoints.push_back(point1);
					cutoff = true;
				}
				if (cutoff)
				{
					tmptris = allstatictriangles[i].cliptrianglestartPoint(allstatictriangles[i], pointnormals, pointpoints, pointnumber);
					dontpush = true;
					break;
				}
				else
				{
					allstatictriangles[i].r += pointnumber.x; allstatictriangles[i].g += pointnumber.y; allstatictriangles[i].b += pointnumber.z;
				}
			}
		}
		if (!dontpush)
		{
			tmptris.push_back(allstatictriangles[i]);
		}

		for (int z = 0; z < tmptris.size(); z++)
		{
			tmptris[z].clampcolor(255);
			pointclippedtriangles.push_back(tmptris[z]);
		}
		/*vector<Triangle3D> tmp = allstatictriangles[i].cliptrianglestart(allstatictriangles[i]);
		for (int x = 0; x < tmp.size(); x++)
		{
			tmp[x].r = color.x; tmp[x].g = color.y; tmp[x].b = color.z;
			clippedtriangles.push_back(tmp[x]);
		}*/
	}
	//cout << allstatictriangles[0].g << "  ";
	//cout << pointclippedtriangles[0].g << endl;
	//cout << allstatictriangles.size() << "  ";
	//cout << pointclippedtriangles.size() << endl;
	for (int i = 0; i < pointclippedtriangles.size(); i++)
	{
		vector<Triangle3D> tmp = pointclippedtriangles[i].cliptrianglestart(pointclippedtriangles[i]);
		for (int x = 0; x < tmp.size(); x++)
		{
			tmp[x].r = pointclippedtriangles[i].r; tmp[x].g = pointclippedtriangles[i].g; tmp[x].b = pointclippedtriangles[i].b;
			clippedtriangles.push_back(tmp[x]);
		}
	}

	sort(clippedtriangles.begin(), clippedtriangles.end(), [](Triangle3D t1, Triangle3D t2)
	{
		//t1.one.w = 1; t1.two.w = 1; t1.three.w = 1;
		//t2.one.w = 1; t2.two.w = 1; t2.three.w = 1;
		t1.Cameratransform();
		t2.Cameratransform();
		float z1 = (t1.one.z + t1.two.z + t1.three.z) / 3.0f;
		float z2 = (t2.one.z + t2.two.z + t2.three.z) / 3.0f;
		return z1 < z2;
	});
	
	clippedtriangles[0].DrawTriangleNoClip(clippedtriangles, 0, 155, 155,0);

}

void CalculateViewPlanes()
{

	/*front plane (0)*/
	Vec4d point1(thepointlights[0].lightposition.x, thepointlights[0].lightposition.y, thepointlights[0].lightposition.z, 1);
	Vec4d point2(point1.x, point1.y, point1.z + 1, 1);
	Vec4d point3(point1.x, point1.y + 1, point1.z, 1);
	Vec4d p1; p1 = point2 - point1;
	Vec4d p2; p2 = point3 - point1;
	Vec4d normal = crossproduct(p1, p2); //normalize?

	Vec4d n(cameralookat.x, cameralookat.y, cameralookat.z, 1);
	Vec4d p(cameralocation.x, cameralocation.y, cameralocation.z, 1);
	p = p - n * .1;//.1
	//n = normal;
	//p = point2;

	/*back plane (0)*/
	Vec4d bn(cameralookat.x, cameralookat.y, cameralookat.z, 1);
	bn = bn * -1;
	
	Vec3d bptmp = cameralocation - (cameralookat * ff);
	Vec4d bp(bptmp.x, bptmp.y, bptmp.z, 1);

	/*top plane (0)*/
	Vec3d tatmp = cameralocation + cameraright * (fr)+cameraup * (ft)+cameralookat * fn;
	Vec4d ta(tatmp.x, tatmp.y, tatmp.z, 1);

	Vec3d tbtmp = cameralocation + cameraright * (fl)+cameraup * (ft)+cameralookat * fn;
	Vec4d tb(tbtmp.x, tbtmp.y, tbtmp.z, 1);

	float nfratio = ff / fn;

	Vec3d tctmp = tatmp + cameraright * nfratio + cameraup * nfratio - cameralookat * nfratio;
	Vec4d tc(tctmp.x, tctmp.y, tctmp.z, 1);

	Vec4d tn = crossproduct(tb - ta, tc - ta);
	Vec4d tp = ta;

	/*bottom plane (0)*/
	Vec3d datmp = cameralocation + cameraright * (fr)+cameraup * (ft)-cameralookat * fn;
	Vec4d da(datmp.x, datmp.y, datmp.z, 1);

	Vec3d dbtmp = cameralocation + cameraright * (fl)+cameraup * (ft)-cameralookat * fn;
	Vec4d db(dbtmp.x, dbtmp.y, dbtmp.z, 1);

	Vec3d dctmp = datmp + cameraright * nfratio + cameraup * nfratio + cameralookat * nfratio;
	Vec4d dc(dctmp.x, dctmp.y, dctmp.z, 1);

	Vec4d dn = crossproduct(db - da, dc - da);
	Vec4d dp = da;

	/*left plane (0)*/
	Vec3d latmp = cameralocation + cameraright * (fl)+cameraup * (ft)+cameralookat * fn;
	Vec4d la(latmp.x, latmp.y, latmp.z, 1);

	Vec3d lbtmp = cameralocation + cameraright * (fl)+cameraup * (fb)+cameralookat * fn;
	Vec4d lb(lbtmp.x, lbtmp.y, lbtmp.z, 1);

	Vec3d lctmp = latmp + cameraright * nfratio + cameraup * nfratio - cameralookat * nfratio;
	Vec4d lc(lctmp.x, lctmp.y, lctmp.z, 1);

	Vec4d ln = crossproduct(lb - la, lc - la);
	Vec4d lp = la;

	/*right plane (0)*/
	Vec3d ratmp = cameralocation + cameraright * (fl * 1) + cameraup * (ft * 1) - cameralookat * fn;
	Vec4d ra(ratmp.x, ratmp.y, ratmp.z, 1);

	Vec3d rbtmp = cameralocation + cameraright * (fl * 1) + cameraup * (fb * 1) - cameralookat * fn;
	Vec4d rb(rbtmp.x, rbtmp.y, rbtmp.z, 1);

	Vec3d rctmp = ratmp + cameraright * nfratio + cameraup * nfratio + cameralookat * nfratio;
	Vec4d rc(rctmp.x, rctmp.y, rctmp.z, 1);

	Vec4d rn = crossproduct(rb - ra, rc - ra);
	Vec4d rp = ra;

	facenormals[0] = n; facenormals[1] = bn; facenormals[2] = ln;
	facenormals[3] = rn; facenormals[4] = tn; facenormals[5] = dn;

	facepoints[0] = p; facepoints[1] = bp; facepoints[2] = lp;
	facepoints[3] = rp; facepoints[4] = tp; facepoints[5] = dp;
}

void CalculateViewPlanes3()
{
	/*front plane (0)*/
	Vec4d n(cameralookat3.x, cameralookat3.y, cameralookat3.z, 1);
	Vec4d p(cameralocation3.x, cameralocation3.y, cameralocation3.z, 1);
	p = p - n * .1;//.1

	/*back plane (0)*/
	Vec4d bn(cameralookat3.x, cameralookat3.y, cameralookat3.z, 1);
	bn = bn * -1;

	Vec3d bptmp = cameralocation3 - (cameralookat3 * ff);
	Vec4d bp(bptmp.x, bptmp.y, bptmp.z, 1);

	/*top plane (0)*/
	Vec3d tatmp = cameralocation3 + cameraright3 * (fr)+cameraup3 * (ft)+cameralookat3 * fn;
	Vec4d ta(tatmp.x, tatmp.y, tatmp.z, 1);

	Vec3d tbtmp = cameralocation3 + cameraright3 * (fl)+cameraup3 * (ft)+cameralookat3 * fn;
	Vec4d tb(tbtmp.x, tbtmp.y, tbtmp.z, 1);

	float nfratio = ff / fn;

	Vec3d tctmp = tatmp + cameraright3 * nfratio + cameraup3 * nfratio - cameralookat3 * nfratio;
	Vec4d tc(tctmp.x, tctmp.y, tctmp.z, 1);

	Vec4d tn = crossproduct(tb - ta, tc - ta);
	Vec4d tp = ta;

	/*bottom plane (0)*/
	Vec3d datmp = cameralocation3 + cameraright3 * (fr)+cameraup3 * (ft)-cameralookat3 * fn;
	Vec4d da(datmp.x, datmp.y, datmp.z, 1);

	Vec3d dbtmp = cameralocation3 + cameraright3 * (fl)+cameraup3 * (ft)-cameralookat3 * fn;
	Vec4d db(dbtmp.x, dbtmp.y, dbtmp.z, 1);

	Vec3d dctmp = datmp + cameraright3 * nfratio + cameraup3 * nfratio + cameralookat3 * nfratio;
	Vec4d dc(dctmp.x, dctmp.y, dctmp.z, 1);

	Vec4d dn = crossproduct(db - da, dc - da);
	Vec4d dp = da;

	/*left plane (0)*/
	Vec3d latmp = cameralocation3 + cameraright3 * (fl)+cameraup3 * (ft)+cameralookat3 * fn;
	Vec4d la(latmp.x, latmp.y, latmp.z, 1);

	Vec3d lbtmp = cameralocation3 + cameraright3 * (fl)+cameraup3 * (fb)+cameralookat3 * fn;
	Vec4d lb(lbtmp.x, lbtmp.y, lbtmp.z, 1);

	Vec3d lctmp = latmp + cameraright3 * nfratio + cameraup3 * nfratio - cameralookat3 * nfratio;
	Vec4d lc(lctmp.x, lctmp.y, lctmp.z, 1);

	Vec4d ln = crossproduct(lb - la, lc - la);
	Vec4d lp = la;

	/*right plane (0)*/
	Vec3d ratmp = cameralocation3 + cameraright3 * (fl * 1) + cameraup3 * (ft * 1) - cameralookat3 * fn;
	Vec4d ra(ratmp.x, ratmp.y, ratmp.z, 1);

	Vec3d rbtmp = cameralocation3 + cameraright3 * (fl * 1) + cameraup3 * (fb * 1) - cameralookat3 * fn;
	Vec4d rb(rbtmp.x, rbtmp.y, rbtmp.z, 1);

	Vec3d rctmp = ratmp + cameraright3 * nfratio + cameraup3 * nfratio + cameralookat3 * nfratio;
	Vec4d rc(rctmp.x, rctmp.y, rctmp.z, 1);

	Vec4d rn = crossproduct(rb - ra, rc - ra);
	Vec4d rp = ra;

	facenormals[0] = n; facenormals[1] = bn; facenormals[2] = ln;
	facenormals[3] = rn; facenormals[4] = tn; facenormals[5] = dn;

	facepoints[0] = p; facepoints[1] = bp; facepoints[2] = lp;
	facepoints[3] = rp; facepoints[4] = tp; facepoints[5] = dp;
}

void CalculateViewPlanes2()
{

	/*front plane (0)*/
	Vec4d n(cameralookat2.x, cameralookat2.y, cameralookat2.z, 1);
	Vec4d p(cameralocation2.x, cameralocation2.y, cameralocation2.z, 1);
	p = p - n * .1;//.1

	/*back plane (0)*/
	Vec4d bn(cameralookat2.x, cameralookat2.y, cameralookat2.z, 1);
	bn = bn * -1;

	Vec3d bptmp = cameralocation2 - (cameralookat2 * ff);
	Vec4d bp(bptmp.x, bptmp.y, bptmp.z, 1);

	/*top plane (0)*/
	Vec3d tatmp = cameralocation2 + cameraright2 * (fr)+cameraup2 * (ft)+cameralookat2 * fn;
	Vec4d ta(tatmp.x, tatmp.y, tatmp.z, 1);

	Vec3d tbtmp = cameralocation2 + cameraright2 * (fl)+cameraup2 * (ft)+cameralookat2 * fn;
	Vec4d tb(tbtmp.x, tbtmp.y, tbtmp.z, 1);

	float nfratio = ff / fn;

	Vec3d tctmp = tatmp + cameraright2 * nfratio + cameraup2 * nfratio - cameralookat2 * nfratio;
	Vec4d tc(tctmp.x, tctmp.y, tctmp.z, 1);

	Vec4d tn = crossproduct(tb - ta, tc - ta);
	Vec4d tp = ta;

	/*bottom plane (0)*/
	Vec3d datmp = cameralocation2 + cameraright2 * (fr)+cameraup2 * (ft)-cameralookat2 * fn;
	Vec4d da(datmp.x, datmp.y, datmp.z, 1);

	Vec3d dbtmp = cameralocation2 + cameraright2 * (fl)+cameraup2 * (ft)-cameralookat2 * fn;
	Vec4d db(dbtmp.x, dbtmp.y, dbtmp.z, 1);

	Vec3d dctmp = datmp + cameraright2 * nfratio + cameraup2 * nfratio + cameralookat2 * nfratio;
	Vec4d dc(dctmp.x, dctmp.y, dctmp.z, 1);

	Vec4d dn = crossproduct(db - da, dc - da);
	Vec4d dp = da;

	/*left plane (0)*/
	Vec3d latmp = cameralocation2 + cameraright2 * (fl)+cameraup2 * (ft)+cameralookat2 * fn;
	Vec4d la(latmp.x, latmp.y, latmp.z, 1);

	Vec3d lbtmp = cameralocation2 + cameraright2 * (fl)+cameraup2 * (fb)+cameralookat2 * fn;
	Vec4d lb(lbtmp.x, lbtmp.y, lbtmp.z, 1);

	Vec3d lctmp = latmp + cameraright2 * nfratio + cameraup2 * nfratio - cameralookat2 * nfratio;
	Vec4d lc(lctmp.x, lctmp.y, lctmp.z, 1);

	Vec4d ln = crossproduct(lb - la, lc - la);
	Vec4d lp = la;

	/*right plane (0)*/
	Vec3d ratmp = cameralocation2 + cameraright2 * (fl * 1) + cameraup2 * (ft * 1) - cameralookat2 * fn;
	Vec4d ra(ratmp.x, ratmp.y, ratmp.z, 1);

	Vec3d rbtmp = cameralocation2 + cameraright2 * (fl * 1) + cameraup2 * (fb * 1) - cameralookat2 * fn;
	Vec4d rb(rbtmp.x, rbtmp.y, rbtmp.z, 1);

	Vec3d rctmp = ratmp + cameraright2 * nfratio + cameraup2 * nfratio + cameralookat2 * nfratio;
	Vec4d rc(rctmp.x, rctmp.y, rctmp.z, 1);

	Vec4d rn = crossproduct(rb - ra, rc - ra);
	Vec4d rp = ra;

	facenormals[0] = n; facenormals[1] = bn; facenormals[2] = ln;
	facenormals[3] = rn; facenormals[4] = tn; facenormals[5] = dn;

	facepoints[0] = p; facepoints[1] = bp; facepoints[2] = lp;
	facepoints[3] = rp; facepoints[4] = tp; facepoints[5] = dp;
}

Vec4d crossproduct(Vec4d one, Vec4d two)
{
	Vec4d p;
	p.x = one.y * two.z - one.z * two.y;
	p.y = one.z * two.x - one.x * two.z;
	p.z = one.x * two.y - one.y * two.x;

	return p;
}

bool checkcollision(Ray r, Triangle3D& tri)
{
	//compute scalar on 3rd tri vertex (y)
	//std::cout << r.getdirection().z << std::endl;
	float tt = raytrihelper(2, r, tri);
	//std::cout << tt << std::endl;
	if (tt < 0 || tt > 10000)
	{
		return false;
	}
	//ts = tt;
	float y = raytrihelper(1, r, tri);
	//cout << y << endl;
	if (y < 0 || y > 1)
	{
		return false;
	}

	//computer scalar on 2nd tri vertex(b)
	float b = raytrihelper(0, r, tri);

	if (b < 0 || b > 1 - y)
	{
		return false;
	}

	return true;
}

float raytrihelper(int y, Ray r, Triangle3D& tri)
{
	float a, b, c, d, e, f, g, h, i, j, k, l;
	a = tri.one.x - tri.two.x;
	b = tri.one.y - tri.two.y;
	c = tri.one.z - tri.two.z;
	d = tri.one.x - tri.three.x;
	e = tri.one.y - tri.three.y;
	f = tri.one.z - tri.three.z;
	g = r.direction.x;
	h = r.direction.y;
	i = r.direction.z;
	j = tri.one.x - r.position.x;
	k = tri.one.y - r.position.y;
	l = tri.one.z - r.position.z;
	//regular determinent of raypointposition = trianglepositions
	float m = a * (e*i - h * f) + b * (g*f - d * i) + c * (d*h - e * g);
	float they;
	if (y == 1)
	{
		they = i * (a*k - j * b) + h * (j*c - a * l) + g * (b*l - k * c);
	}
	else if (y == 0)
	{
		they = j * (e*i - h * f) + k * (g*f - d * i) + l * (d*h - e * g);
	}
	else
	{
		they = f * (a*k - j * b) + e * (j*c - a * l) + d * (b*l - k * c);
		they = -(they / m);
		return they;
	}
	they = (they / m);
	return they;
}

void LoadOBJVNT(Mesh& meshname, string filename, bool negativetroll)
{
	vector<vector<float>> vertices;
	vector<vector<float>> normals;
	vector<Triangle3D> triangles;

	float x, y, z, u, v;
	int vone, vtwo, vthree, tone, ttwo, tthree, none, ntwo, nthree;
	vector<float> tmp;
	//Triangle3D tmptri;
	fstream file;
	string holder;
	file.open(filename, std::ios::in);
	bool leave = false;
	while (!file.eof())
	{
		if (file.peek() == 118) //118 is v
		{
			getline(file, holder, 'v');
			char peek = file.peek();
			if (peek == ' ')//vertex position
			{
				while (file.peek() == ' ')
				{
					file.get();
				}
				getline(file, holder, ' ');
				x = stof(holder);
				getline(file, holder, ' ');
				y = stof(holder);
				getline(file, holder);
				z = stof(holder);
				
				tmp.push_back(x); tmp.push_back(y); tmp.push_back(z);// tmp.push_back(0); tmp.push_back(0);
				vertices.push_back(tmp);
				tmp.clear();
			}
			else if (peek == 'n')//normal position
			{
				file.get(); //take the n out
				while (file.peek() == ' ')
				{
					file.get();
				}
				getline(file, holder, ' ');
				x = stof(holder);
				getline(file, holder, ' ');
				y = stof(holder);
				getline(file, holder);
				z = stof(holder);
				//cout << "n: " << x << " " << y << " " << z << endl;
				tmp.push_back(x); tmp.push_back(y); tmp.push_back(z);
				normals.push_back(tmp);
				tmp.clear();
			}
			else
			{
				getline(file, holder);
				//cout << "t: " << holder << endl;
			}
		}
		else
		{
			getline(file, holder);
		}
	}
	//std::cout << vertices.size() << " " << std::endl;
	//std::cout << normals.size() << " " << std::endl;

	file.close();
	file.open(filename, std::ios::in);
	leave = false;
	while (!file.eof())
	{
		char a = file.peek();
		//cout << a << endl;
		if (a == 102) // 102 is f key
		{
			file.get(); //get rid of f
			while (file.peek() == ' ')
			{
				file.get();
			}
			getline(file, holder, '/');
			vone = stoi(holder);
			getline(file, holder, '/');
			//tone = stoi(holder);
			getline(file, holder, ' ');
			none = stoi(holder);

			getline(file, holder, '/');
			vtwo = stoi(holder);
			getline(file, holder, '/');
			//ttwo = stoi(holder);
			getline(file, holder, ' ');
			ntwo = stoi(holder);

			getline(file, holder, '/');
			vthree = stoi(holder);
			getline(file, holder, '/');
			//tthree = stoi(holder);
			getline(file, holder, '\n');
			nthree = stoi(holder);

			if (negativetroll)
			{
				cout << "negative troll" << endl;
				vone = -vone; vtwo = -vtwo; vthree = -vthree;
				none = -none; ntwo = -ntwo; nthree = -nthree;
			}
			
			//cout << none << " " << ntwo << " " << nthree << endl;
			Vec4d theone(vertices[vone - 1][0], vertices[vone - 1][1], vertices[vone - 1][2], 1);
			Vec4d thetwo(vertices[vtwo - 1][0], vertices[vtwo - 1][1], vertices[vtwo - 1][2], 1);
			Vec4d thethree(vertices[vthree - 1][0], vertices[vthree - 1][1], vertices[vthree - 1][2], 1);
			Vec4d normal1(normals[none - 1][0], normals[none - 1][1], normals[none - 1][2], 1);
			Vec4d normal2(normals[ntwo - 1][0], normals[ntwo - 1][1], normals[ntwo - 1][2], 1);
			Vec4d normal3(normals[nthree - 1][0], normals[nthree - 1][1], normals[nthree - 1][2], 1);
			Vec4d normal; normal = (normal1 + normal2 + normal3) / 3;
			//cout << normal.x << " " << normal.y << " " << normal.z << endl;
			Triangle3D tmptriangle(theone, thetwo, thethree, normal);
			triangles.push_back(tmptriangle);
			tmp.clear();
		}
		else
		{
			getline(file, holder);
		}
	}
	cout << filename << ": " << triangles.size() << endl;
	for (int i = 0; i < triangles.size(); i++)
	{
		meshname.AddTriangle(triangles[i]);
	}
	file.close();
}

void LoadOBJVT(Mesh& meshname, string filename)
{
	vector<vector<float>> vertices;
	vector<Triangle3D> triangles;

	float x, y, z, u, v;
	int vone, vtwo, vthree, tone, ttwo, tthree, none, ntwo, nthree;
	vector<float> tmp;
	//Triangle3D tmptri;
	fstream file;
	string holder;
	file.open(filename, std::ios::in);
	bool leave = false;
	while (!file.eof())
	{
		if (file.peek() == 118) //118 is v
		{
			getline(file, holder, 'v');
			char peek = file.peek();
			if (peek == ' ')//vertex position
			{
				while (file.peek() == ' ')
				{
					file.get();
				}
				getline(file, holder, ' ');
				x = stof(holder);
				getline(file, holder, ' ');
				y = stof(holder);
				getline(file, holder);
				z = stof(holder);

				tmp.push_back(x); tmp.push_back(y); tmp.push_back(z);// tmp.push_back(0); tmp.push_back(0);
				vertices.push_back(tmp);
				tmp.clear();
			}
			else
			{
				getline(file, holder);
				//cout << "t: " << holder << endl;
			}
		}
		else
		{
			getline(file, holder);
		}
	}
	//std::cout << vertices.size() << " " << std::endl;
	//std::cout << normals.size() << " " << std::endl;

	file.close();
	file.open(filename, std::ios::in);
	leave = false;
	while (!file.eof())
	{
		char a = file.peek();
		//cout << a << endl;
		if (a == 102) // 102 is f key
		{
			file.get(); //get rid of f
			while (file.peek() == ' ')
			{
				file.get();
			}
			getline(file, holder, '/');
			vone = stoi(holder);
			getline(file, holder, ' ');
			//tone = stoi(holder);

			getline(file, holder, '/');
			vtwo = stoi(holder);
			getline(file, holder, ' ');
			//ttwo = stoi(holder);

			getline(file, holder, '/');
			vthree = stoi(holder);
			getline(file, holder, '\n');
			//tthree = stoi(holder);


			//cout << none << " " << ntwo << " " << nthree << endl;
			Vec4d theone(vertices[vone - 1][0], vertices[vone - 1][1], vertices[vone - 1][2], 1);
			Vec4d thetwo(vertices[vtwo - 1][0], vertices[vtwo - 1][1], vertices[vtwo - 1][2], 1);
			Vec4d thethree(vertices[vthree - 1][0], vertices[vthree - 1][1], vertices[vthree - 1][2], 1);
			Triangle3D tmptriangle(theone, thetwo, thethree);
			triangles.push_back(tmptriangle);
			tmp.clear();
		}
		else
		{
			getline(file, holder);
		}
	}
	cout << filename << ": " << triangles.size() << endl;
	for (int i = 0; i < triangles.size(); i++)
	{
		meshname.AddTriangle(triangles[i]);
	}
	file.close();
}

bool boxboxcollision(Box a, Box b)
{
	//The sides of the rectangles
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;
	float frontB, backB;
	float frontA, backA;

	//Calculate the sides of rect A
	leftA = a.one.x;
	rightA = a.two.x;
	topA = a.one.y;
	bottomA = a.three.y;
	frontA = a.one.z;
	backA = a.five.z;

	//Calculate the sides of rect B
	leftB = b.one.x;
	rightB = b.two.x;
	topB = b.one.y;
	bottomB = b.three.y;
	frontB = b.one.z;
	backB = b.five.z;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	if (backA <= frontB)
	{
		return false;
	}

	if (frontA >= backB)
	{
		return false;
	}

	return true;
}

bool playermoveback(Box boxa, Box boxb, Vec3d thetmp)
{
	if (boxboxcollision(box2, boxb))
	{
		cameralocation.x = cameralocation.x - thetmp.x;
		ak47.Move(-thetmp.x, 0, 0);
		Grunt.Move(-thetmp.x, 0, 0);
		box2.Move(-thetmp.x, 0, 0);
		if (boxboxcollision(box2, boxb))
		{
			//move back x move y
			cameralocation.x = cameralocation.x + thetmp.x;
			cameralocation.y = cameralocation.y - thetmp.y;
			ak47.Move(thetmp.x, 0, 0);
			Grunt.Move(thetmp.x, 0, 0);
			box2.Move(thetmp.x, 0, 0);
			ak47.Move(0, -thetmp.y, 0);
			Grunt.Move(0, -thetmp.y, 0);
			box2.Move(0, -thetmp.y, 0);
			if (boxboxcollision(box2, boxb))
			{
				//move back y, we know its z
				cameralocation.y = cameralocation.y + thetmp.y;
				cameralocation.z = cameralocation.z - thetmp.z;
				ak47.Move(0, thetmp.y, 0);
				Grunt.Move(0, thetmp.y, 0);
				box2.Move(0, thetmp.y, 0);
				ak47.Move(0, 0, -thetmp.z);
				Grunt.Move(0, 0, -thetmp.z);
				box2.Move(0, 0, -thetmp.z);
			}
			else
			{
				//hit y were done
			}
		}
		else
		{
			//hit x were done
		}
		return true;
	}
	return false;
}