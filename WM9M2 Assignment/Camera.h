#pragma once

#include "Mathlib.h"

using namespace Mathlib;

class Camera {
public:

	Vec3 from;
	Vec3 to;
	Vec3 up;
	Matrix view;

	void init(Vec3 _from, Vec3 _to, Vec3 _up) {
		from = _from;
		to = _to;
		up = _up;
		view = Matrix::LookAt(from, to, up);
	}

	void changetoSet(int index, float time) {

		switch (index) {
		case 1:
		{
			from = Vec3(10 * cosf(time), 5, 10 * sinf(time));
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

		view = Matrix::LookAt(from, to, up);

	}



};