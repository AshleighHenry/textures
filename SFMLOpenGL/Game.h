#ifndef GAME_H
#define GAME_H

#include <Debug.h>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <Vector3.h>
#include <Matrix3.h>
#include <MyVector3.h>

#include <InitialMyMatrix3.h>





#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace sf;
//using namespace gpp;

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();
	void initalizeVertex();

	Clock clock;
	Time elapsed;
	float m_scale = 1.f;
	float m_rotationAngleX = 1.f;
	float m_rotationAngleY = 1.f;
	float m_rotationAngleZ = 1.f;
	MyVector3 m_displacmentVector{ 0,0,0 };
};

#endif