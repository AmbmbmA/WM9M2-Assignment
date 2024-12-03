#pragma once

#include "Mathlib.h"
#include "DXcore.h"
#include "Shaders.h"

using namespace Mathlib;

struct Vertex
{
	Vec3 position;
	Colour colour;

};

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;

	// texture coordinate
	float tu;
	float tv;
};

struct ANIMATED_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;

	// texture coordinate
	float tu;
	float tv;


	unsigned int bonesIDs[4]; // vertex influenced by 4 bones
	float boneWeights[4]; // weigt of influence of the 4 bones
};

static STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv)
{
	STATIC_VERTEX v;
	v.pos = p;
	v.normal = n;
	//Frame frame;
	//frame.fromVector(n);
	//v.tangent = frame.u; // For now
	v.tangent = Vec3(0, 0, 0); // For now
	v.tu = tu;
	v.tv = tv;
	return v;
}

class Mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	int indicesSize; // num of index
	UINT strides; // size of the vertex info     

	~Mesh() {
		if (indexBuffer) indexBuffer->Release();
		if (vertexBuffer) vertexBuffer->Release();
	}

	void init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices);

	// initialize using vector of vertices
	template <typename Vertex>
	void init(DXcore* core, std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		init(core, vertices.data(), sizeof(Vertex), vertices.size(), indices.data(), indices.size());
	}

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp);

};

class Plane {
public:

	Mesh mesh;

	void init(DXcore* core);

};

class Cube {
public:

	Mesh mesh;

	void init(DXcore* core);

};


class Sphere {
public:

	Mesh mesh;

	void init(DXcore* core, int rings, int segments, float radius);

};





