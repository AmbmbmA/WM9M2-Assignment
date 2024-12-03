#pragma once

#include "Mathlib.h"
#include "Window.h"

using namespace Mathlib;

class Camera {
private:
	void updatevp() {

		Sphericalcoor s(theta * M_PI / 180, phi * M_PI / 180, 1);
		Vec3 face = s.convertcartesian();
		to = position + face;
		up = Vec3(0, 1, 0);

		movedirforward = Vec3(face.x, 0, face.z).normalize();
		movedirright = movedirforward.Crossright(Vec3(0, 1, 0)).normalize();

		view = Matrix::LookAt(position, to, up);
		vp = p * view;
	}

	void checkanglelimit() {
		if (theta >= 180) theta = 179.99f;
		if (theta <= 0) theta = 0.01f;

		phi = fmod(phi, 360.0f);
		if (phi < 0) phi += 360.0f;
	}

	enum keypress { w, a, s, d };
public:

	Vec3 position;
	float theta; // rotate around x
	float phi; // rotate around y
	float r;

	Vec3 to;
	Vec3 up;
	Vec3 movedirforward;
	Vec3 movedirright;

	bool direcmoniter[4] = { false,false,false,false };

	Matrix view;
	Matrix p;
	Matrix vp;
	Window* win;


	void init(Vec3 _position, float _theta, float _phi, Matrix _p, Window* _win) {
		position = _position;
		theta = _theta;
		phi = _phi;
		checkanglelimit();
		p = _p;
		updatevp();
		win = _win;
	}

	void move() {

		float sensity = 0.01;

		Vec3 temp = Vec3(0, 0, 0);
		if (direcmoniter[0]) {
			temp += movedirforward;
		}
		if (direcmoniter[1]) {
			temp -= movedirright;
		}
		if (direcmoniter[2]) {
			temp -= movedirforward;
		}
		if (direcmoniter[3]) {
			temp += movedirright;
		}
		Vec3 a = temp.normalize();
		position = position + (temp.normalize() * sensity);

	}

	void rotate(float dtheta, float dphi) {
		theta += dtheta;
		phi += dphi;

		checkanglelimit();
	}

	void changetoSet(int index, float time) {

		switch (index) {
		case 1:
		{
			position = Vec3(10 * cosf(time), 10, 10 * sinf(time));
			to = Vec3(0, 0, 0);
			up = Vec3(0, 1, 0);
			break;
		}
		case 2:
		{

		}
		default:
		{
			return;
		}
		}

		updatevp();

	}

	void update() {



		float sensity = 0.3;
		rotate(win->rawmousey * sensity, win->rawmousex * sensity);


		if (win->keys['W']) {
			direcmoniter[0] = true;
		}
		if (win->keys['A']) {
			direcmoniter[1] = true;
		}
		if (win->keys['S']) {
			direcmoniter[2] = true;
		}
		if (win->keys['D']) {
			direcmoniter[3] = true;
		}
		if (direcmoniter[0] || direcmoniter[1] || direcmoniter[2] || direcmoniter[3]) move();
		updatevp();
		direcmoniter[0] = direcmoniter[1] = direcmoniter[2] = direcmoniter[3] = false;

	}



};