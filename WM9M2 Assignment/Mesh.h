#pragma once

#include "Mathlib.h"
#include "DXcore.h"
#include "Shaders.h"
#include "GEMLoader.h"
#include "Animation.h"
#include "Texture.h"
#include "Collision.h"

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

static STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv,int tiling)
{
	STATIC_VERTEX v;
	v.pos = p;
	v.normal = n;
	//Frame frame;
	//frame.fromVector(n);
	//v.tangent = frame.u; // For now
	v.tangent = Vec3(0, 0, 0); // For now
	v.tu = tu * tiling;
	v.tv = tv * tiling;
	return v;
}



class Mesh {
public:
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;

	unsigned int strides[2];
	ID3D11Buffer* buffers[2];

	int indicesSize; // num of index
	int instancenum;

	void init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, vector<Vec3> instanceData,int _instancenum);

	// initialize using vector of vertices
	template <typename Vertex>
	void init(DXcore* core, std::vector<Vertex> vertices, std::vector<unsigned int> indices, vector<Vec3> instanceData, int _instancenum) {
		init(core, vertices.data(), sizeof(Vertex), vertices.size(), indices.data(), indices.size(), instanceData, _instancenum);
	}

	void updateinstanceBuffer(DXcore* core, vector<Vec3>newinstancedata);

	void draw(DXcore* core);

	void free() {
		if (indexBuffer) indexBuffer->Release();
		if (vertexBuffer) vertexBuffer->Release();

	}
};

class Plane {
public:

	Mesh mesh;
	~Plane() { mesh.free(); }

	void init(DXcore* core,int tiling, vector<Vec3> instanceData, int instancenum);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures, string texturefilename);



};

class Cube {
public:

	Mesh mesh;

	~Cube() { mesh.free(); }

	void init(DXcore* core, int tiling,vector<Vec3> instanceData, int instancenum);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures, string texturefilename);

};

class Sphere {
public:

	Mesh mesh;

	~Sphere() { mesh.free(); }

	void init(DXcore* core, int tiling, int rings, int segments, float radius, vector<Vec3> instanceData, int instancenum);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures, string texturefilename);

};




class StaticModel {
public:

	std::vector<Mesh> meshes;
	std::vector<std::string> textureFilenames;
	std::vector<std::string> normalFilenames;

	~StaticModel() {
		for (auto m : meshes) {
			m.free();
		}
	}

	void init(DXcore* core, string filename, vector<Vec3> instanceData, int instancenum,AABB* box);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures);

};

class StaticModelwithtiling {
public:

	std::vector<Mesh> meshes;
	std::vector<std::string> textureFilenames;

	~StaticModelwithtiling() {
		for (auto m : meshes) {
			m.free();
		}
	}

	void init(DXcore* core, string filename,int tilingnum, vector<Vec3> instanceData, int instancenum, AABB* box);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures,string tf, string nf);

};

// list animation names in a log
static void listAnimationNames(const GEMLoader::GEMAnimation& gemanimation)
{
	ofstream write;
	write.open("Log.txt", ios::app);
	if (!write.is_open()) {
		write.open("Log.txt", ios::out);
	}
	else
	{
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			write << gemanimation.animations[i].name << std::endl;
		}
	}

	write.close();

}


class AnimatedModel {
public:

	std::vector<Mesh> meshes;
	Animation animation;

	std::vector<std::string> textureFilenames;

	~AnimatedModel() {
		for (auto m : meshes) {
			m.free();
		}
	}

	void init(DXcore* core, string filename, vector<Vec3> instanceData, int instancenum);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, AnimationInstance* instance, TextureManager* textures);

};




