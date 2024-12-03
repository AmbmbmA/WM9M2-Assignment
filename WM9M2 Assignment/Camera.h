#pragma once

#include "Mathlib.h"
#include "Window.h"

using namespace Mathlib;

class Camera {
private:
	void updatevp();

	void checkanglelimit();

public:

	// for lookat Matrix
	Vec3 position;
	Vec3 to;
	Vec3 up;

	// for view direction moving
	float theta; // rotate around x
	float phi; // rotate around y

	// direction for camera moving
	Vec3 movedirforward;
	Vec3 movedirright;
	bool direcmoniter[4] = { false,false,false,false }; // moniter weather this is input for moving

	Matrix view; // view matrix
	Matrix p; // projection
	Matrix vp;

	Window* win;// to get input

	void init(Vec3 _position, float _theta, float _phi, Matrix _p, Window* _win);

	void move(float dt);

	void rotate(float dtheta, float dphi);

	void update(float dt);

};