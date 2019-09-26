#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
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
#include "BSP.h"
#include <time.h>
using namespace std::chrono;
using namespace std;

int screenwidth = 1000;
int screenheight = 550;
SDL_Window* gwindow = 0;
SDL_Renderer* grender = 0;
SDL_Surface* gsurface = 0;

/*Frostum*/
float fl = -1;
float fr = 1;
float ft = -1;
float fb = 1;
float fn = 1;
float ff = 50;

Vec4d facenormals[6];
Vec4d facepoints[6];
vector<Triangle3D> clippedtriangles;

float lastmousex;
float lastmousey;
float mousechangex;
float mousechangey;

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

Vec3d crossproduct(Vec3d one, Vec3d two);

Vec3d cameralocation(0, 0, 0);
Vec3d cameralookat(0, 0, -1);
Vec3d cameraup(0, 1, 0);
Vec3d cameraright(1, 0, 0);
Vec4d cameralocation4d;
float pitch = 90.0f;
float yaw = 270.0f;
vector<Triangle3D> allstatictriangles;
bool slow = false;

void LoadOBJ(Mesh& meshname, string filename);
void LoadSpaceShip(Mesh& meshname, string filename);
void ClockWiseArc();
void EllipseBresenhamsMidpoint();
void CircleTrivial();
void CircleBresenhamsMidPoint();
void LineBresenhams();
Vec4d crossproduct(Vec4d one, Vec4d two);
void LineDDA();
bool init();
float toradians(float degrees);
void updatecamera();
Vec3d normalize(Vec3d one);
void RenderedSortedTriangle();
void CalculateViewPlanes();
int main(int argc, char* argv[])
{
	init();
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
	LoadOBJ(Scarab, "scarab.txt");
	//LoadOBJ(Ghost, "ghost.txt");
	LoadOBJ(MasterChief, "masterchief.txt");
	LoadOBJ(EnergySword, "energysword.txt");
	LoadOBJ(CrashB, "crashb.txt");

	LoadSpaceShip(TeaPot, "teapot.txt");
	LoadSpaceShip(Mountains, "mountains.txt");
	LoadSpaceShip(videoship, "axis.txt");
	LoadSpaceShip(building, "globe.txt");

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
	//cout << thetriangle.Triangles[0].two.x << "  " << thetriangle.Triangles[1].two.x << endl;
	thetriangle.SetNormals();

	Vec4d fronttopleft(.8 + xcounter, -1.5 + ycounter, 1 + zcounter, 1);
	Vec4d fronttopright(3 + xcounter, -1.5 + ycounter, 1 + zcounter, 1);
	Vec4d frontbottomleft(.8 + xcounter, -.3 + ycounter, 1 + zcounter, 1);
	Vec4d frontbottomright(3 + xcounter, -.3 + ycounter, 1 + zcounter, 1);
	Vec4d backtopleft(.8 + xcounter, -1.5 + ycounter, 5.5 + zcounter, 1);
	Vec4d backtopright(3 + xcounter, -1.5 + ycounter, 5.5 + zcounter, 1);
	Vec4d backbottomleft(.8 + xcounter, -.3 + ycounter, 5.5 + zcounter, 1);
	Vec4d backbottomright(3 + xcounter, -.3 + ycounter, 5.5 + zcounter, 1);
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

	//EnergySword.Scale(.05, .05, .05);
	EnergySword.Rotate(.03, 0, .19, EnergySword.Triangles[0].one); //.064, 5
	EnergySword.Move(1, 1,-1.2);

	Vec4d cheifcenter(0, 0, 0, 1);
	MasterChief.Rotate(.80, 30, 0, cheifcenter);
	//MasterChief.Scale(.1, .1, .05);

	CrashB.Scale(.1, .1, .1);
	CrashB.Rotate(0, 2.14, 2.14, CrashB.Triangles[0].one);

	TeaPot.Rotate(.6, 0, 0, cheifcenter);
	TeaPot.Move(10, 0, 0);
	TeaPot.Scale(2, 2, 2);

	//SpaceShip.Scale(2, 2, 2);
	//SpaceShip.Rotate(0, 0.02010, 0, tricenter);
	
	Vec4d mountaincenter(0, 0, 0, 1);
	Mountains.Scale(.7, .7, .7);
	Mountains.Rotate(.6, 0, 0, mountaincenter);

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
		allstatictriangles.push_back(x);
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

	BSP bsp1;
	auto startbsp = high_resolution_clock::now();
	bsp1.CreateTree(allstatictriangles);
	auto stopbsp = high_resolution_clock::now();
	auto durationbsp = duration_cast<microseconds>(stopbsp - startbsp);
	cout << "bsp creation time: " << durationbsp.count() << "ms" << endl;

	float fps = 0;
	Vec4d playercenter(0, 0, 0, 1);
	float cameraspeed = 1.0f;
	int acounter = 0;
	while (exit == false)
	{
		auto start = high_resolution_clock::now();
		mousechangex = 0;
		mousechangey = 0;
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
				float xamount, yamount, zamount;
				float amount = .001;
				xamount = .1;
				yamount = .1;
				zamount = .1;
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:	break;
				case SDLK_BACKSPACE:break;
				case SDLK_s: break;//cameralocation = cameralocation - cameralookat; break;// zcounter += zamount;     thetriangle.add(0, 0, zamount);	thesquare.add(0, 0, zamount);	SpaceShip.add(0, 0, zamount);	MasterChief.add(0, 0, zamount);	 EnergySword.add(0, 0, zamount);	CrashB.add(0, 0, zamount);	tricenter.z += zamount; squarecenter.z += zamount;  break;
				case SDLK_w: break;//cameralocation = cameralocation + cameralookat; break;//zcounter -= zamount;     thetriangle.add(0, 0, -zamount);	thesquare.add(0, 0, -zamount);	SpaceShip.add(0, 0, -zamount);	MasterChief.add(0, 0, -zamount);  EnergySword.add(0, 0, -zamount);  CrashB.add(0, 0, -zamount); tricenter.z += -zamount; squarecenter.z += -zamount; break;
				case SDLK_a:break;// cameralocation = cameralocation - cameraright; break;// xcounter -= xamount;     thetriangle.add(-xamount, 0, 0);	thesquare.add(-xamount, 0, 0);	SpaceShip.add(-xamount, 0, 0);	MasterChief.add(-xamount, 0, 0);  EnergySword.add(-xamount, 0, 0); CrashB.add(-xamount, 0, 0); tricenter.x += -xamount; squarecenter.x += -xamount; break;
				case SDLK_d: break;//cameralocation = cameralocation + cameraright; break;// xcounter += xamount;     thetriangle.add(xamount, 0, 0);	thesquare.add(xamount, 0, 0);	SpaceShip.add(xamount, 0, 0);	MasterChief.add(xamount, 0, 0);	 EnergySword.add(xamount, 0, 0); 	CrashB.add(xamount, 0, 0); tricenter.x += xamount; squarecenter.x += xamount;  break;
				case SDLK_SPACE: break;//cameralocation = cameralocation - cameraup; break; //ycounter -= yamount; thetriangle.add(0, -yamount, 0);  thesquare.add(0, -yamount, 0);  SpaceShip.add(0, -yamount, 0);  MasterChief.add(0, -yamount, 0);  EnergySword.add(0, -yamount, 0); CrashB.add(0, -yamount, 0); tricenter.y += -yamount; squarecenter.y += -yamount; break;
				case SDLK_LCTRL:break; //cameralocation = cameralocation + cameraup; break;// ycounter += yamount; thetriangle.add(0, yamount, 0);	thesquare.add(0, yamount, 0);	SpaceShip.add(0, yamount, 0);	MasterChief.add(0, yamount, 0);  EnergySword.add(0, yamount, 0);   CrashB.add(0, yamount, 0);  tricenter.y += yamount; squarecenter.y += yamount;  break;
				case SDLK_LEFT:   yaw += cameraspeed; 		updatecamera(); break;//SpaceShip.Rotate(0, amount, 0, playercenter); thetriangle.Rotate(0, amount, 0, playercenter); thesquare.Rotate(0, amount, 0, playercenter); MasterChief.Rotate(0, amount, 0, playercenter);     EnergySword.Rotate(0, amount, 0, playercenter); CrashB.Rotate(0, amount, 0, playercenter);   break;
				case SDLK_RIGHT:   yaw -= cameraspeed; updatecamera(); break;//SpaceShip.Rotate(0, -amount, 0, playercenter); thetriangle.Rotate(0, -amount, 0, playercenter); thesquare.Rotate(0, -amount, 0, playercenter); MasterChief.Rotate(0, -amount, 0, playercenter); EnergySword.Rotate(0, -amount, 0, playercenter); CrashB.Rotate(0, -amount, 0, playercenter);  break;
				case SDLK_UP:     pitch += cameraspeed; updatecamera(); break;//SpaceShip.Rotate(amount, 0, 0, playercenter); thetriangle.Rotate(amount, 0, 0, playercenter); thesquare.Rotate(amount, 0, 0, playercenter); MasterChief.Rotate(amount, 0, 0, playercenter);      EnergySword.Rotate(amount, 0, 0, playercenter); CrashB.Rotate(amount, 0, 0, playercenter);   break;
				case SDLK_DOWN:    pitch -= cameraspeed; updatecamera(); break;//SpaceShip.Rotate(-amount, 0, 0, playercenter); thetriangle.Rotate(-amount, 0, 0, playercenter); thesquare.Rotate(-amount, 0, 0, playercenter); MasterChief.Rotate(-amount, 0, 0, playercenter);  EnergySword.Rotate(-amount, 0, 0, playercenter); CrashB.Rotate(-amount, 0, 0, playercenter);  break;
				case SDLK_ESCAPE:  exit = true; break;
				case SDLK_b: slow = !slow; break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
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
			
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				
			}

		}
	/*				Main Loop						*/

		const Uint8* currentkeystate = SDL_GetKeyboardState(0);
		float turnspeed = 0.3f;

		yaw -= mousechangex * turnspeed;
		pitch += mousechangey * turnspeed;
		updatecamera();

		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_WarpMouseInWindow(gwindow, (screenwidth / 2), (screenheight/2));
		/*if (x <= (screenwidth/2) - 100)
		{
			SDL_WarpMouseInWindow(gwindow, (screenwidth/2) + 100, y);
			lastmousex = (screenwidth / 2) + 100;
		}
		if (x >= (screenwidth / 2) + 100)
		{
			SDL_WarpMouseInWindow(gwindow, (screenwidth / 2) - 100, y);
			lastmousex = (screenwidth / 2) - 100;
		}
		if (y <= 0)
		{
			SDL_WarpMouseInWindow(gwindow, x, screenheight - 2);
			lastmousey = screenheight - 2;
		}
		if (y > screenheight - 2)
		{
			SDL_WarpMouseInWindow(gwindow, x, 1);
			lastmousey = 1;
		}*/

		float movementspeed = 0.1f;
		if (currentkeystate[SDL_SCANCODE_A])
		{
			Vec3d tmp(cameraright.x*movementspeed, cameraright.y*movementspeed, cameraright.z*movementspeed);
			cameralocation = cameralocation + tmp;
		}
		if (currentkeystate[SDL_SCANCODE_D])
		{
			Vec3d tmp(cameraright.x*movementspeed, cameraright.y*movementspeed, cameraright.z*movementspeed);
			cameralocation = cameralocation - tmp;
		}
		if (currentkeystate[SDL_SCANCODE_S])
		{
			Vec3d tmp(cameralookat.x*movementspeed, cameralookat.y*movementspeed, cameralookat.z*movementspeed);
			cameralocation = cameralocation + tmp;
		}
		if (currentkeystate[SDL_SCANCODE_W])
		{
			Vec3d tmp(cameralookat.x*movementspeed, cameralookat.y*movementspeed, cameralookat.z*movementspeed);
			cameralocation = cameralocation - tmp;
		}
		if (currentkeystate[SDL_SCANCODE_LCTRL])
		{
			Vec3d tmp(cameraup.x*movementspeed, cameraup.y*movementspeed, cameraup.z*movementspeed);
			cameralocation = cameralocation - tmp;
		}
		if (currentkeystate[SDL_SCANCODE_SPACE])
		{
			Vec3d tmp(cameraup.x*movementspeed, cameraup.y*movementspeed, cameraup.z*movementspeed);
			cameralocation = cameralocation + tmp;
		}

		SDL_SetRenderDrawColor(grender, 255, 255, 255, 0);
		SDL_RenderClear(grender);

		/*Object Transform*/
		//thetriangle.Rotate(0, 0.00001, 0, tricenter);
		//thesquare.Rotate(0.00001, 0.00001, 0.00001, squarecenter);
		SpaceShip.Rotate(0, 0.00010, 0, tricenter);
		//MasterChief.Rotate(0, 0.001, 0, MasterChief.Triangles[0].one);
		//EnergySword.Rotate(0, .001, 0, EnergySword.Triangles[0].one);
		//CrashB.Rotate(0, .001, 0, CrashB.Triangles[0].one);
		
		
		CalculateViewPlanes();
		cameralocation4d.x = cameralocation.x; cameralocation4d.y = cameralocation.y; cameralocation4d.z = cameralocation.z; cameralocation4d.w = 1;

		/*clipping slowest thing by far*/
		///*BSP RENDERING*/
		Vec4d e(cameralocation.x, cameralocation.y, cameralocation.z, 1);
		bsp1.DrawStart(grender, e,slow);


		
		/*SORTED TRIANGLE RENDERING*/
		//RenderedSortedTriangle();
	
		
		/*PER OBJECT RENDERING*/
		//Mesh tmptriangle = thetriangle;
		//tmptriangle.Print(255, 0, 0);

		//Mesh tmpsquare = thesquare;
		//tmpsquare.Print(255, 0, 0);

		//Mesh tmpspaceship = SpaceShip;
		//tmpspaceship.Print(0, 255, 0);

		//Mesh tmpcheif = MasterChief;
		//tmpcheif.Print(0, 0, 255);
		
		//Mesh tmpenergysword = EnergySword;
		//tmpenergysword.Print(0, 0, 255);

		//Mesh tmpcrash = CrashB;
		//tmpcrash.Print(255, 165, 0);

		//Mesh tmpScarab = Scarab;
		//tmpScarab.Print(0, 255, 0);

		acounter++;

		if (acounter == 50)
		{
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//cout << "fps: " << 1 / (duration.count() / 1000000.0f) << endl;
			//1/(duration.count()/1000000.0f)
			acounter = 0;
			fps = 1 / (duration.count() / 1000000.0f);
		}
		int fpsint = fps;
		string fpsstring = to_string(fpsint);
		const char* lol = fpsstring.c_str();
		SDL_SetWindowTitle(gwindow, lol);

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
		}
	}
	return success;
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


	//cout<< cameralookat.dotproduct(cameraright)<<" "<< cameralookat.dotproduct(cameraup)<< " "<< cameraright.dotproduct(cameraup)<<endl;
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

void RenderedSortedTriangle()
{
	allstatictriangles.clear();

	/*for (Triangle3D x : thetriangle.Triangles)
	{
		allstatictriangles.push_back(x);
	}
	for (Triangle3D x : thesquare.Triangles)
	{
		allstatictriangles.push_back(x);
	}
	for (Triangle3D x : SpaceShip.Triangles)
	{
		allstatictriangles.push_back(x);
	}*/
	for (Triangle3D x : CrashB.Triangles)
	{
		allstatictriangles.push_back(x);
	}
	for (Triangle3D x : TeaPot.Triangles)
	{
		allstatictriangles.push_back(x);
	}

	sort(allstatictriangles.begin(), allstatictriangles.end(), [](Triangle3D t1, Triangle3D t2)
	{
		t1.Cameratransform();
		t2.Cameratransform();
		float z1 = (t1.one.z + t1.two.z + t1.three.z) / 3.0f;
		float z2 = (t2.one.z + t2.two.z + t2.three.z) / 3.0f;
		return z1 < z2;
	});

	for (int i = 0; i < allstatictriangles.size(); i++)
	{
		allstatictriangles[i].DrawTriangle(0, 0, 0);
		if (slow)
		{
			SDL_RenderPresent(grender);
			SDL_Delay(50);
		}
	}
}

void CalculateViewPlanes()
{

	/*front plane (0)*/
	Vec4d n(cameralookat.x, cameralookat.y, cameralookat.z, 1);
	Vec4d p(cameralocation.x, cameralocation.y, cameralocation.z, 1);
	p = p - n * .1;//.1

	/*back plane (0)*/
	Vec4d bn(cameralookat.x, cameralookat.y, cameralookat.z, 1);
	bn = bn * -1;
	Vec3d bptmp = cameralocation - cameralookat * ff;
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

Vec4d crossproduct(Vec4d one, Vec4d two)
{
	Vec4d p;
	p.x = one.y * two.z - one.z * two.y;
	p.y = one.z * two.x - one.x * two.z;
	p.z = one.x * two.y - one.y * two.x;

	return p;
}