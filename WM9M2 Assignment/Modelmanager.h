#pragma once

#include "Mathlib.h"
#include "Mesh.h"
#include "DXcore.h"
#include "Shaders.h"

using namespace Mathlib;

#include <vector>
using namespace std;

class OBJ {
public:
	vector<Vec3>  instanceposition;

	Matrix W = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));
    Vec3 position;     
    Vec3 scale;         
    float health;      


    virtual bool checkCollision(const OBJ& other) const {

        Vec3 minA = position - scale * 0.5f;
        Vec3 maxA = position + scale * 0.5f;
        Vec3 minB = other.position - other.scale * 0.5f;
        Vec3 maxB = other.position + other.scale * 0.5f;

        return (minA.x <= maxB.x && maxA.x >= minB.x &&
            minA.y <= maxB.y && maxA.y >= minB.y &&
            minA.z <= maxB.z && maxA.z >= minB.z);
    }

};


class StaticOBJ : OBJ {
private:
    StaticModel model;



};



class AnimatedOBJ : OBJ {



};






