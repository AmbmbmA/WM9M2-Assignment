#pragma once

#include "Mathlib.h"
#include "DXcore.h"
#include "Shaders.h"
#include "GEMLoader.h"

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



	void init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices);

	// initialize using vector of vertices
	template <typename Vertex>
	void init(DXcore* core, std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		init(core, vertices.data(), sizeof(Vertex), vertices.size(), indices.data(), indices.size());
	}

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp);

	void free() {
		if (indexBuffer) indexBuffer->Release();
		if (vertexBuffer) vertexBuffer->Release();
	}
};

class Plane {
public:

	Mesh mesh;

	~Plane() { mesh.free(); }
	void init(DXcore* core);

};

class Cube {
public:

	Mesh mesh;

	~Cube() { mesh.free(); }

	void init(DXcore* core);

};

class Sphere {
public:

	Mesh mesh;

	~Sphere() { mesh.free(); }

	void init(DXcore* core, int rings, int segments, float radius);

};

class Model {
public:

	std::vector<Mesh> meshes;

	~Model() {
		for (auto m : meshes) {
			m.free();
		}
	}

	void init(DXcore* core, string filename) {
		GEMLoader::GEMModelLoader loader;

		std::vector<GEMLoader::GEMMesh> gemmeshes;

		loader.load(filename, gemmeshes);

		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh mesh;
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			mesh.init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
	}

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp) {

		Matrix Scaledworld = World->Scaling(Vec3(0.01f, 0.01f, 0.01f));

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(core, shader, &Scaledworld, vp);
		}

	}


};



