#pragma once

#include "Mathlib.h"
#include "Window.h"
#include "Collision.h"
#include <fstream>

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

	float mousesensity = 100;
	float speed = 30;

	// for view direction moving
	float theta; // rotate around x
	float phi; // rotate around y

	float thetasmooth = 0.0f;
	float phismooth = 0.0f;

	// direction for camera moving
	Vec3 movedirforward;
	Vec3 movedirright;
	bool direcmoniter[4] = { false,false,false,false }; // moniter weather this is input for moving

	bool isjump = false;
	float jumpspeed = 30;
	float jumpspeedtemp = 0;

	float health = 1000;


	Matrix view; // view matrix
	Matrix p; // projection
	Matrix vp;

	Window* win;// to get input

	void init(Vec3 _position, float _theta, float _phi, Matrix _p, Window* _win);

	void move(float dt);

	void rotate(double dtheta, double dphi);

	void update(float dt);

	void jump();

	void save(ofstream& save) {
		save << health << endl;
		save << position.x << endl;
		save << position.y << endl;
		save << position.z << endl;
		save << theta << endl;
		save << phi << endl;

	}

	void load(ifstream& load) {
		load >> health;
		float x, y, z;
		load >> x;
		load >> y;
		load >> z;
		position = Vec3(x, y, z);
		load >> theta;
		load >> phi;
	}
};