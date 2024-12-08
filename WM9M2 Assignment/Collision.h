#pragma once

#include "Mathlib.h"

using namespace Mathlib;


class Ray {
public:
	Vec3 origin;
	Vec3 direction;
	float distance;

	Ray() {
		origin = Vec3(0, 0, 0);
		direction = Vec3(0, 0, 0);
		distance = 0;
	}

	void set(Vec3 _origin, Vec3 _direction, float _distance) {
		origin = _origin;
		direction = _direction;
		distance = _distance;
	}
};

class SphereColl
{
public:
	Vec3 centre;
	float radius;

	SphereColl() {
		centre = Vec3(0, 0, 0);
		radius = 0;
	}

	void set(Vec3 _centre, float _radius) {
		centre = _centre;
		radius = _radius;
	}


};

class AABB
{
public:
	Vec3 max;
	Vec3 min;
	AABB() { reset(); }
	void reset()
	{
		max = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	}
	void extend(const Vec3& p)
	{
		max = Max(max, p);
		min = Min(min, p);
	}

};




class Collision {
public:

	Ray ray;

	SphereColl sphere1;
	SphereColl sphere2;

	AABB box1;
	AABB box2;


	void setray(Vec3 _origin, Vec3 _direction, float _distance) { ray.set(_origin, _direction, _distance); }

	void setsphere1(Vec3 _centre, float _radius) { sphere1.set(_centre, _radius); }
	void setsphere2(Vec3 _centre, float _radius) { sphere2.set(_centre, _radius); }

	void setbox1(AABB _box) { box1 = _box; }
	void setbox2(AABB _box) { box2 = _box; }

	bool raysphere1check() {

		Vec3 L = ray.origin - sphere1.centre;

		float b = ray.direction.normalize().Dot(L);
		float c = L.getlengthsquare() - square(sphere1.radius);

		float delta = square(b) - c;

		if (delta <= 0) {
			return false;
		}


		float t1 = -b + sqrtf(delta);
		float t2 = -b - sqrtf(delta);

		if (t1 >= 0 && t2 <= ray.distance) {
			return true;
		}

		return false;
	}

	bool raybox1check() {

		return false;

	}

	bool box1sphere1check() {


		return false;

	}

	bool boxcheck() {



		return false;

	}

	bool spherecheck(Vec3& contra) {

		Vec3 different = sphere1.centre - sphere2.centre;
		float dsq = different.getlengthsquare();
		float radiumsum = sphere1.radius + sphere2.radius;
		float rsq = square(radiumsum);

		float overlap = radiumsum - different.getlength();

		contra = different.normalize() * overlap;

		if (dsq <= rsq) {
			return true;
		}



		return false;

	}




};
