#include "Camera.h"


void Camera::updatevp() {

	Sphericalcoor s(theta * M_PI / 180, phi * M_PI / 180, 1);
	Vec3 face = s.convertcartesian();
	to = position + face;
	up = Vec3(0, 1, 0);

	thirdposition = position + Vec3(0, 3, 0);
	thirdposition = position - movedirforward * 2;

	firstposition = position;

	movedirforward = Vec3(face.x, 0, face.z).normalize();
	movedirright = movedirforward.Crossright(Vec3(0, 1, 0)).normalize();

	view = Matrix::LookAt(position, to, up);
	vp = p * view;
}

void Camera::checkanglelimit() {
	if (theta >= 180) theta = 179.99f;
	if (theta <= 0) theta = 0.01f;

	phi = fmod(phi, 360.0f);
	if (phi < 0) phi += 360.0f;
}

void Camera::init(Vec3 _position, float _theta, float _phi, Matrix _p, Window* _win) {
	position = _position;
	theta = _theta;
	phi = _phi;
	checkanglelimit();
	p = _p;
	updatevp();
	win = _win;
}

void Camera::move(float sensity) {

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

void Camera::rotate(float dtheta, float dphi) {

	theta += dtheta;
	phi += dphi;

	checkanglelimit();
}

void Camera::update(float dt) {

	float sensity = 0.2 + 0.2 * sin(100 * dt);

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
	if (direcmoniter[0] || direcmoniter[1] || direcmoniter[2] || direcmoniter[3]) move(0.05*sensity);

	updatevp();

	direcmoniter[0] = direcmoniter[1] = direcmoniter[2] = direcmoniter[3] = false;

}