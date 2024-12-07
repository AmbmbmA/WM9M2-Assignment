#include "Camera.h"


void Camera::updatevp() {

	Sphericalcoor s(theta * M_PI / 180, phi * M_PI / 180, 1);
	Vec3 face = s.convertcartesian();
	to = position + face;
	up = Vec3(0, 1, 0);

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

void Camera::rotate(double dtheta, double dphi) {

	theta += dtheta;
	phi += dphi;

	checkanglelimit();
}

void Camera::jump() {
	if (!isjump) {
		isjump = true;
		jumpspeedtemp = jumpspeed;
	}
}


void Camera::update(float dt) {

	float sensity = 150 * dt;

	rotate(win->rawmousey * 0.5 * sensity, win->rawmousex * sensity);

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
	if (direcmoniter[0] || direcmoniter[1] || direcmoniter[2] || direcmoniter[3]) move(0.2 * sensity);

	if (isjump) {
		float g = 10 * 9.8 * dt;
		jumpspeedtemp -= g; // 重力作用
		position.y += jumpspeedtemp * dt; // 更新位置
		if (position.y <= 8) {
			position.y = 8;
			isjump = false;
			jumpspeedtemp = 0;
		}
	}


	updatevp();

	direcmoniter[0] = direcmoniter[1] = direcmoniter[2] = direcmoniter[3] = false;

	float g = 9.8 * dt;
	float v = 5;

}