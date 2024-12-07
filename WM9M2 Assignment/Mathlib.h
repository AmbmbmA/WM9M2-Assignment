#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <memory.h>
#include <string>

#ifndef M_PI
#define M_PI       3.14159265358979323846   
#endif 


using namespace std;

#define square(x) (x) * (x)

#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)

namespace Mathlib {
	// return Linear Interpolation between a and b
	template<typename T>
	static T lerp(const T a, const T b, float t)
	{
		return a * (1.0f - t) + (b * t);
	}

	class Vec2
	{
	public:
		union
		{
			float v[2];
			struct { float x, y; };
		};

		Vec2() {
			for (int i = 0; i < 2; i++) {
				v[i] = 0;
			}
		}

		Vec2(float _x, float _y) :x(_x), y(_y) {

		}

		Vec2 operator+(const Vec2& pVec) const
		{
			return Vec2(v[0] + pVec.v[0], v[1] + pVec.v[1]);
		}
		Vec2& operator+=(const Vec2& pVec)
		{
			v[0] += pVec.v[0];
			v[1] += pVec.v[1];
			return *this;
		}
		Vec2 operator-(const Vec2& pVec) const
		{
			return Vec2(v[0] - pVec.v[0], v[1] - pVec.v[1]);
		}
		Vec2& operator-=(const Vec2& pVec)
		{
			v[0] -= pVec.v[0];
			v[1] -= pVec.v[1];
			return *this;
		}
		Vec2 operator*(const Vec2& pVec) const
		{
			return Vec2(v[0] * pVec.v[0], v[1] * pVec.v[1]);
		}
		Vec2& operator*= (const Vec2& pVec)
		{
			v[0] *= pVec.v[0];
			v[1] *= pVec.v[1];
			return *this;
		}
		Vec2 operator*(const float val) const
		{
			return Vec2(v[0] * val, v[1] * val);
		}
		Vec2& operator*=(const float val)
		{
			v[0] *= val;
			v[1] *= val;
			return *this;
		}
		Vec2 operator/(const Vec2& pVec)const {
			return Vec2(x / pVec.x, y / pVec.y);
		}
		Vec2& operator/=(const Vec2 pVec) {
			x /= pVec.x;
			y /= pVec.y;
			return *this;
		}
		Vec2 operator/(const float val)const {
			return Vec2(x / val, y / val);
		}
		Vec2& operator/=(const float val) {
			x /= val;
			y /= val;
			return *this;
		}
		Vec2 operator-() const
		{
			return Vec2(-v[0], -v[1]);
		}

		float getlength() const {

			return sqrtf(square(x) + square(y));

		}
		float getlengthsquare() const {

			return square(x) + square(y);
		}
		Vec2 normalize() const
		{
			float len = 1.0f / sqrtf(square(x) + square(y));
			return Vec2(x * len, y * len);
		}
		float normalize_GetLength()
		{
			float length = sqrtf(square(x) + square(y));
			float len = 1.0f / length;
			v[0] *= len; v[1] *= len;
			return length;
		}

		float Dot(const Vec2& pVec) const
		{
			return v[0] * pVec.v[0] + v[1] * pVec.v[1];
		}
		float Angle(const Vec2& v2) {
			return acosf((x * v2.x + y * v2.y) / (sqrtf(square(x) + square(y)) * sqrtf(square(v2.x) + square(v2.y))));
		}
		float Crossleft(const Vec2& v1)const
		{
			return v1.x * y - v1.y * x;
		}
		float Crossright(const Vec2& v2)const
		{
			return x * v2.y - y * v2.x;
		}

		void Printconsole()const {
			cout << "(" << x << ", " << y << ")" << endl;
		}
		string Getvectorstring() const {

			return "(" + to_string(x) + ", " + to_string(y) + ")";

		}

		float Maxcom() {
			return max(x, y);
		}
		Vec2 Max(const Vec2& v2) const {
			return Vec2(max(x, v2.x), max(y, v2.y));
		}
		float Mincom() {
			return min(x, y);
		}
		Vec2 Min(const Vec2& v2) const {
			return Vec2(min(x, v2.x), min(y, v2.y));
		}

	};

	class Vec3
	{
	public:
		union
		{
			float v[3];
			struct { float x, y, z; };
		};

		Vec3() {
			for (int i = 0; i < 3; i++) {
				v[i] = 0;
			}
		}

		Vec3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {

		}

		Vec3 operator+(const Vec3& pVec) const
		{
			return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
		}
		Vec3& operator+=(const Vec3& pVec)
		{
			v[0] += pVec.v[0];
			v[1] += pVec.v[1];
			v[2] += pVec.v[2];
			return *this;
		}
		Vec3 operator-(const Vec3& pVec) const
		{
			return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
		}
		Vec3& operator-=(const Vec3& pVec)
		{
			v[0] -= pVec.v[0];
			v[1] -= pVec.v[1];
			v[2] -= pVec.v[2];
			return *this;
		}
		Vec3 operator*(const Vec3& pVec) const
		{
			return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
		}
		Vec3& operator*= (const Vec3& pVec)
		{
			v[0] *= pVec.v[0];
			v[1] *= pVec.v[1];
			v[2] *= pVec.v[2];
			return *this;
		}
		Vec3 operator*(const float val) const
		{
			return Vec3(v[0] * val, v[1] * val, v[2] * val);
		}
		Vec3& operator*=(const float val)
		{
			v[0] *= val;
			v[1] *= val;
			v[2] *= val;
			return *this;
		}
		Vec3 operator/(const Vec3& pVec)const {
			return Vec3(x / pVec.x, y / pVec.y, z / pVec.z);
		}
		Vec3& operator/=(const Vec3 pVec) {
			x /= pVec.x;
			y /= pVec.y;
			z /= pVec.z;
			return *this;
		}
		Vec3 operator/(const float val)const {
			return Vec3(x / val, y / val, z / val);
		}
		Vec3& operator/=(const float val) {
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}
		Vec3 operator-() const
		{
			return Vec3(-v[0], -v[1], -v[2]);
		}

		void NDCmaptoscreen(const float width, const float height, int& pixelx, int& pixely) const {
			pixelx = (x + 1) * width / 2;
			pixely = (y + 1) * height / 2;
		}

		float getlength() const {

			return sqrtf(square(x) + square(y) + square(z));

		}
		float getlengthsquare() const {

			return square(x) + square(y) + square(z);
		}
		Vec3 normalize() const
		{
			float len = 1.0f / sqrtf(square(x) + square(y) + square(z));
			if (x == 0 && y == 0 && z == 0) len = 0;
			return Vec3(x * len, y * len, z * len);
		}
		float normalize_GetLength()
		{
			float length = sqrtf(square(x) + square(y) + square(z));
			float len = 1.0f / length;
			v[0] *= len; v[1] *= len; v[2] *= len;
			return length;
		}

		float Dot(const Vec3& pVec) const
		{
			return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
		}
		float Angle(const Vec3& v2) {
			return acosf((x * v2.x + y * v2.y + z * v2.z) / (sqrtf(square(x) + square(y) + square(z)) * sqrtf(square(v2.x) + square(v2.y) + square(v2.z))));
		}
		Vec3 Crossleft(const Vec3& v1)const
		{
			return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
				v1.v[2] * v[0] - v1.v[0] * v[2],
				v1.v[0] * v[1] - v1.v[1] * v[0]);
		}
		Vec3 Crossright(const Vec3& v2)const
		{
			return Vec3(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
		}

		void Printconsole()const {
			cout << "(" << x << ", " << y << ", " << z << ")" << endl;
		}
		string Getvectorstring() const {

			return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";

		}

		float Maxcom() {
			return max(x, max(y, z));
		}
		Vec3 Max(const Vec3& v2) const {
			return Vec3(max(x, v2.x), max(y, v2.y), max(z, v2.z));
		}
		float Mincom() {
			return min(x, min(y, z));
		}
		Vec3 Min(const Vec3& v2) const {
			return Vec3(min(x, v2.x), min(y, v2.y), min(z, v2.z));
		}


	};

	class Vec4
	{
	public:
		union
		{
			float v[4];
			struct { float x, y, z, w; };
		};

		Vec4() {
			for (int i = 0; i < 4; i++) {
				v[i] = 0;
			}
		}

		Vec4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {

		}

		Vec4(Vec3& v3, float _w) :w(_w) {
			x = v3.x;
			y = v3.y;
			z = v3.z;
		}

		Vec3 getVec3() const {
			return Vec3(x, y, z);
		}

		Vec4 perspectivedivide() {
			x /= w;
			y /= w;
			z /= w;
			w = 1 / w;
			return *this;
		}

		Vec4 getperspectivedivide()const {

			return Vec4(x / w, y / w, z / w, 1 / w);
		}

	};

	static float dot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static float angle(const Vec3& v1, const Vec3& v2) {
		return acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / (sqrtf(square(v1.x) + square(v1.y) + square(v1.z)) * sqrtf(square(v2.x) + square(v2.y) + square(v2.z))));
	}
	static Vec3 cross(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	static Vec3 Max(const Vec3& v1, const Vec3& v2) {
		return Vec3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
	}
	static Vec3 Min(const Vec3& v1, const Vec3& v2) {
		return Vec3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
	}

	class Matrix
	{
	public:
		union
		{
			float a[4][4];
			float m[16];
		};

		Matrix() {
			identity();
		}
		Matrix(float* otherMatrix) {
			memcpy(&m, otherMatrix, 16 * sizeof(float));
		}
		void identity() {
			memset(m, 0, 16 * sizeof(float));
			m[0] = 1.0f;
			m[5] = 1.0f;
			m[10] = 1.0f;
			m[15] = 1.0f;
		}

		float& operator[](int index) {
			return m[index];
		}
		Matrix operator*(const Matrix& matrix)
		{
			return mul(matrix);
		}


		Vec3 mulPoint(const Vec3& v) const
		{
			Vec3 v1 = Vec3(
				(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
				(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
				(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
			float w;
			w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
			w = 1.0f / w;
			return (v1 * w);
		}
		Vec3 mulVec(const Vec3& v) const
		{
			return Vec3(
				(v.x * m[0] + v.y * m[1] + v.z * m[2]),
				(v.x * m[4] + v.y * m[5] + v.z * m[6]),
				(v.x * m[8] + v.y * m[9] + v.z * m[10]));
		}

		Vec4 mulVec4(const Vec4& v)const {
			return Vec4(
				(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3]),
				(v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7]),
				(v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11]),
				(v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]));
		}

		static Matrix Translation(const Vec3& v) {
			Matrix result;
			result.m[3] = v.x;
			result.m[7] = v.y;
			result.m[11] = v.z;
			return result;
		}
		static Matrix Scaling(const Vec3& v) {
			Matrix result;
			result.m[0] = v.x;
			result.m[5] = v.y;
			result.m[10] = v.z;
			return result;
		}
		static Matrix RotationX(const float& theta) {
			Matrix result;
			float cost = cosf(theta);
			float sint = sinf(theta);
			result.m[5] = cost;
			result.m[6] = -sint;
			result.m[9] = sint;
			result.m[10] = cost;
			return result;
		}
		static Matrix RotationY(const float& theta) {
			Matrix result;
			float cost = cosf(theta);
			float sint = sinf(theta);
			result.m[0] = cost;
			result.m[2] = sint;
			result.m[8] = -sint;
			result.m[10] = cost;
			return result;
		}
		static Matrix RotationZ(const float& theta) {
			Matrix result;
			float cost = cosf(theta);
			float sint = sinf(theta);
			result.m[0] = cost;
			result.m[1] = -sint;
			result.m[4] = sint;
			result.m[5] = cost;
			return result;
		}
		static Matrix RotationAroundAxis(const Vec3& axis, float angle) {
			Vec3 normalizedAxis = axis.normalize();

			float x = normalizedAxis.x;
			float y = normalizedAxis.y;
			float z = normalizedAxis.z;

			float cosTheta = cos(angle);
			float sinTheta = sin(angle);
			float oneMinusCos = 1.0f - cosTheta;

			Matrix rotation;

			rotation.m[0] = cosTheta + x * x * oneMinusCos;
			rotation.m[1] = x * y * oneMinusCos - z * sinTheta;
			rotation.m[2] = x * z * oneMinusCos + y * sinTheta;
			rotation.m[3] = 0;

			rotation.m[4] = y * x * oneMinusCos + z * sinTheta;
			rotation.m[5] = cosTheta + y * y * oneMinusCos;
			rotation.m[6] = y * z * oneMinusCos - x * sinTheta;
			rotation.m[7] = 0;

			rotation.m[8] = z * x * oneMinusCos - y * sinTheta;
			rotation.m[9] = z * y * oneMinusCos + x * sinTheta;
			rotation.m[10] = cosTheta + z * z * oneMinusCos;
			rotation.m[11] = 0;

			rotation.m[12] = 0;
			rotation.m[13] = 0;
			rotation.m[14] = 0;
			rotation.m[15] = 1;

			return rotation;
		}
		static Matrix Transformationto(const Vec3& u, const Vec3& n, const Vec3& v, const Vec3& p) {
			Matrix result;
			result.m[0] = u.x;
			result.m[1] = u.y;
			result.m[2] = u.z;
			result.m[3] = p.Dot(u);
			result.m[4] = n.x;
			result.m[5] = n.y;
			result.m[6] = n.z;
			result.m[7] = p.Dot(n);
			result.m[8] = v.x;
			result.m[9] = v.y;
			result.m[10] = v.z;
			result.m[11] = p.Dot(v);
			return result;
		}
		static Matrix Transformationback(const Vec3& u, const Vec3& n, const Vec3& v, const Vec3& p) {
			Matrix result;
			result.m[0] = u.x;
			result.m[4] = u.y;
			result.m[8] = u.z;
			result.m[1] = n.x;
			result.m[5] = n.y;
			result.m[9] = n.z;
			result.m[2] = v.z;
			result.m[6] = v.y;
			result.m[10] = v.z;
			result.m[3] = p.x;
			result.m[7] = p.y;
			result.m[11] = p.z;
			return result;
		}
		static Matrix ViewMatrix() {

		}
		static Matrix LookAt(const Vec3& from, const Vec3& to, const Vec3& up) {
			Vec3 dir = (from - to).normalize();
			Vec3 right = (up.Crossright(dir)).normalize();
			Vec3 upn = dir.Crossright(right);
			Matrix result;
			result.m[0] = right.x;
			result.m[1] = right.y;
			result.m[2] = right.z;
			result.m[3] = -from.Dot(right);
			result.m[4] = upn.x;
			result.m[5] = upn.y;
			result.m[6] = upn.z;
			result.m[7] = -from.Dot(upn);
			result.m[8] = dir.x;
			result.m[9] = dir.y;
			result.m[10] = dir.z;
			result.m[11] = -from.Dot(dir);
			return result;
		}
		static Matrix Perspectiveprojectionz01(const float& n, const float& f, const float& fov, const float& aspect) {
			Matrix result;
			auto t = tanf(fov * M_PI / 360);
			result.m[0] = 1 / (aspect * t);
			result.m[5] = 1 / t;
			result.m[10] = -f / (f - n);
			result.m[11] = -f * n / (f - n);
			result.m[14] = -1;
			result.m[15] = 0;

			return result;
		}
		static Matrix Perspectiveprojectionz11(const int& n, const int& f, const float& fov, const float& aspect) {
			Matrix result;
			float t = tanf(fov * M_PI / 360);
			result.m[0] = 1 / (aspect * t);
			result.m[5] = 1 / t;
			result.m[10] = (-f - n) / (f - n);
			result.m[11] = -2 * f * n / (f - n);
			result.m[14] = -1;
			result.m[15] = 0;

			return result;
		}
		static Matrix OrthographicProjection(const int& n, const int& f, const float& width, const float& height) {
			Matrix result;
			result.m[0] = 2 / width;
			result.m[5] = 2 / height;
			result.m[10] = 1 / (f - n);
			result.m[11] = -n / (f - n);

			return result;
		}


		Matrix mul(const Matrix& matrix) const
		{
			Matrix ret;
			ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
			ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
			ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
			ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
			ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
			ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
			ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
			ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
			ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
			ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
			ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
			ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
			ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
			ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
			ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
			ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
			return ret;
		}
		Matrix mulleft(const Matrix& matrix) const
		{
			Matrix ret;
			ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
			ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
			ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
			ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
			ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
			ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
			ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
			ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
			ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
			ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
			ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
			ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
			ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
			ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
			ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
			ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
			return ret;
		}
		Matrix Transpose() {
			float temp;
			temp = m[1]; m[1] = m[4]; m[4] = temp;
			temp = m[2]; m[2] = m[8]; m[8] = temp;
			temp = m[3]; m[3] = m[12]; m[12] = temp;
			temp = m[6]; m[6] = m[9]; m[9] = temp;
			temp = m[7]; m[7] = m[13]; m[13] = temp;
			temp = m[11]; m[11] = m[14]; m[14] = temp;
		}
		Matrix invert()const
		{
			Matrix inv; //fist get the transpose of cofactor matrix
			inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
			float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];// because is transposed
			if (det == 0) {

				cout << "Matrix has determinant = 0 , is singular. No change made." << endl;
				return *this;
			}
			det = 1.0 / det;
			for (int i = 0; i < 16; i++) { // devided by determinant to get inverse
				inv[i] = inv[i] * det;
			}
			return inv;
		}
		float Determinant() const {
			float cof0 = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			float cof1 = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			float cof2 = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			float cof3 = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			return m[0] * cof0 + m[1] * cof1 + m[2] * cof2 + m[3] * cof3;
		}


	};


	class Sphericalcoor {
	public:
		float theta;
		float phi;
		float r;
		bool yup = true; // true y-up, false z-up

		Sphericalcoor(float _theta, float _phi, float _r) : theta(_theta), phi(_phi), r(_r) {}

		Sphericalcoor(const Vec3& v, bool _yup) :yup(_yup) {
			r = v.getlength();
			if (yup) {
				theta = acosf(v.y / r);
				phi = atanf(v.z / v.x);
			}
			else {
				theta = acosf(v.z / r);
				phi = atanf(v.y / v.x);
			}

		}

		Vec3 convertcartesian() const {
			Vec3 result;
			result.x = r * sinf(theta) * cosf(phi);
			if (yup) {
				result.y = r * cosf(theta);
				result.z = r * sinf(theta) * sinf(phi);
			}
			else {
				result.y = r * sinf(theta) * sinf(phi);
				result.z = r * cosf(theta);
			}
			return result;
		}

	};


	template<typename T>
	T BarycentricPerspectiveCorrectInterpolation(T a0, T a1, T a2, float p0w, float p1w, float p2w, float alpha, float beta, float gamma)
	{
		float deno = alpha * p0w + beta * p1w + gamma * p2w;
		T A[3];

		A[0] = a0 * p0w * alpha;
		A[1] = a1 * p1w * beta;
		A[2] = a2 * p2w * gamma;
		return ((A[0] + A[1] + A[2]) / deno);
	}

	class Quaternion {
	public:
		union {
			struct {
				float a;
				float b;
				float c;
				float d;
			};
			float q[4];
		};

		Quaternion() {
			for (int i = 0; i < 4; i++) {
				q[i] = 0;
			}
		}

		Quaternion(float _a, float _b, float _c, float _d) :a(_a), b(_b), c(_c), d(_d) {

		}


		Quaternion operator+(const Quaternion& q) const {
			return Quaternion(a + q.a, b + q.b, c + q.c, d + q.d);
		}
		Quaternion& operator+=(const Quaternion& q) {
			a += q.a;
			b += q.b;
			c += q.c;
			d += q.d;
			return *this;
		}
		Quaternion operator-(const Quaternion& q) const {
			return Quaternion(a - q.a, b - q.b, c - q.c, d - q.d);
		}
		Quaternion& operator-=(const Quaternion& q) {
			a -= q.a;
			b -= q.b;
			c -= q.c;
			d -= q.d;
			return *this;
		}
		Quaternion operator*(const Quaternion& q) const {
			return Quaternion(
				a * q.a - b * q.b - c * q.c - d * q.d,
				a * q.b + b * q.a + c * q.d - d * q.c,
				a * q.c - b * q.d + c * q.a + d * q.b,
				a * q.d + b * q.c - c * q.b + d * q.a
			);
		}
		Quaternion& operator*=(const Quaternion& q) {

			float ta = a * q.a - b * q.b - c * q.c - d * q.d;
			float tb = a * q.b + b * q.a + c * q.d - d * q.c;
			float tc = a * q.c - b * q.d + c * q.a + d * q.b;
			float td = a * q.d + b * q.c - c * q.b + d * q.a;
			a = ta;
			b = tb;
			c = tc;
			d = td;
			return *this;
		}
		Quaternion operator*(const float& val) const {
			return Quaternion(a * val, b * val, c * val, d * val);
		}
		Quaternion& operator*=(const float& val) {
			a *= val;
			b *= val;
			c *= val;
			d *= val;
			return *this;
		}
		Quaternion operator/(const float& val) const {
			return Quaternion(a / val, b / val, c / val, d / val);
		}
		Quaternion& operator/=(const float& val) {
			a /= val;
			b /= val;
			c /= val;
			d /= val;
			return *this;
		}
		Quaternion operator-() const {
			return Quaternion(-a, -b, -c, -d);
		}

		float Dot(const Quaternion& q) const
		{
			return a * q.a + b * q.b + c * q.c + d * q.d;
		}
		float Angle(const Quaternion& q) {
			return acosf(a * q.a + b * q.b + c * q.c + d * q.d);
		}

		float getlength() const {
			return sqrtf(square(a) + square(b) + square(c) + square(d));
		}
		float getlengthsquare() const {

			return square(a) + square(b) + square(c) + square(d);
		}
		Quaternion normalize() const {
			float len = 1.0f / sqrtf(square(a) + square(b) + square(c) + square(d));
			if (len == 0) {
				return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
			}
			return Quaternion(a * len, b * len, c * len, d * len);
		}
		float normalize_GetLength()
		{
			float length = sqrtf(square(a) + square(b) + square(c) + square(d));
			float len = 1.0f / length;
			a *= len; b *= len; c *= len; d *= len;
			return length;
		}
		Quaternion conjugate() const {
			return Quaternion(a, -b, -c, -d);
		}
		Quaternion inverse()const {
			float length = sqrtf(square(a) + square(b) + square(c) + square(d));
			return Quaternion(a / length, -b / length, -c / length, -d / length);
		}

		Matrix toRotationMatrix() const {
			//Matrix result;
			//result.m[0] = 1 - 2 * (square(c) + square(d));
			//result.m[1] = 2 * b * c - 2 * a * d;
			//result.m[2] = 2 * b * d + 2 * a * c;
			//result.m[4] = 2 * b * c + 2 * a * d;
			//result.m[5] = 1 - 2 * (square(b) + square(d));
			//result.m[6] = 2 * c * d - 2 * a * b;
			//result.m[8] = 2 * b * d - 2 * a * c;
			//result.m[9] = 2 * c * d + 2 * a * b;
			//result.m[10] = 1 - 2 * (square(b) + square(c));
			//return result;
			float xx = q[0] * q[0];
			float xy = q[0] * q[1];
			float xz = q[0] * q[2];
			float yy = q[1] * q[1];
			float zz = q[2] * q[2];
			float yz = q[1] * q[2];
			float wx = q[3] * q[0];
			float wy = q[3] * q[1];
			float wz = q[3] * q[2];
			Matrix matrix;
			matrix[0] = 1.0f - 2.0f * (yy + zz);
			matrix[1] = 2.0f * (xy - wz);
			matrix[2] = 2.0f * (xz + wy);
			matrix[3] = 0.0;
			matrix[4] = 2.0f * (xy + wz);
			matrix[5] = 1.0f - 2.0f * (xx + zz);
			matrix[6] = 2.0f * (yz - wx);
			matrix[7] = 0.0;
			matrix[8] = 2.0f * (xz - wy);
			matrix[9] = 2.0f * (yz + wx);
			matrix[10] = 1.0f - 2.0f * (xx + yy);
			matrix[11] = 0.0;
			matrix[12] = 0;
			matrix[13] = 0;
			matrix[14] = 0;
			matrix[15] = 1;
			return matrix;
		}

		static Quaternion Slerp(const Quaternion q1, const Quaternion q2, float t)
		{

			float dot = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
			Quaternion q1m = q1;
			if (dot < 0) {
				dot = -dot;
				q1m = -q1;
			}

			if (dot > 0.99f) {
				//return lerp(q1m, q2, t);
				return (q1m * (1.0f - t) + q2 * t).normalize();
			}

			float theta = acosf(dot);
			float s = sinf(theta);
			float s1 = (sinf(theta * (1 - t))) / s;
			float s2 = (sinf(theta * t)) / s;

			return ((q1m * s1) + (q2 * s2)).normalize();

		}


		static Quaternion Rotate(const Vec3& v, const float theta) {
			Quaternion result;
			float cs = cosf(theta / 2);
			float s = sinf(theta / 2);
			result.a = cs;
			result.b = s * v.x;
			result.c = s * v.y;
			result.d = s * v.z;
			return result;
		}

		static Matrix RotateMatrix(const Vec3& v, const float theta) {
			Quaternion q;
			float cs = cosf(theta / 2);
			float s = sinf(theta / 2);
			q.a = cs;
			q.b = s * v.x;
			q.c = s * v.y;
			q.d = s * v.z;
			Matrix result = q.toRotationMatrix();
			return result;
		}

	};


	class Shadingframe
	{
	public:
		Vec3 u;
		Vec3 v;
		Vec3 n;

		Shadingframe(Vec3& _n) {
			n = _n.normalize();
			Vec3 temp(1, 0, 0);
			if (abs(n.Dot(temp)) > 0.9999f) {
				temp = Vec3(0, 1, 0);
			}
			u = n.Crossright(temp);
			u = u.normalize();
			v = n.Crossright(u);
			v = v.normalize();
		}

		Vec3 Forwardtolocal(const Vec3 wp)const {
			return Vec3(wp.Dot(u), wp.Dot(v), wp.Dot(n));
		}
		Vec3 Inversetoworld(const Vec3 p) {
			return Vec3(p.x * u.x + p.y * v.x + p.z * n.x, p.x * u.y + p.y * v.y + p.z * n.y, p.x * u.z + p.y * v.z + p.z * n.z);
		}

	};

	class Colour {
	public:
		float r;
		float g;
		float b;
		float a;

		Colour() {
			r = 0;
			g = 0;
			b = 0;
			a = 1;
		}

		Colour(float _r, float _g, float _b, float _a = 1) :r(_r), g(_g), b(_b), a(_a) {}

		Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) {
			r = _r / 255.f;
			g = _g / 255.f;
			b = _b / 255.f;
			a = _a / 255.f;
		}

		Colour operator+(const Colour& c) const
		{
			return Colour(r + c.r, g + c.g, b + c.b, a);
		}
		Colour& operator+=(const Colour& c)
		{
			r += c.r;
			g += c.g;
			b += c.b;
			return *this;
		}
		Colour operator-(const Colour& c) const
		{
			return Colour(r - c.r, g - c.g, b - c.b);
		}
		Colour& operator-=(const Colour& c)
		{
			r -= c.r;
			g -= c.g;
			b -= c.b;
			return *this;
		}
		Colour operator*(const Colour& c)const {
			return Colour(r * c.r, g * c.g, b * c.b, a);
		}
		Colour& operator*=(const Colour& c) {
			r *= c.r;
			g *= c.g;
			b *= c.b;
			return *this;
		}
		Colour operator*(const float val)const {
			return Colour(r * val, g * val, b * val, a);
		}
		Colour& operator*=(const float val) {
			r *= val;
			g *= val;
			b *= val;
			return *this;
		}
		Colour operator/(const float val)const {
			return Colour(r / val, g / val, b / val, a);
		}
		Colour& operator/=(const float val) {
			r /= val;
			g /= val;
			b /= val;
			return *this;
		}

	};
}




