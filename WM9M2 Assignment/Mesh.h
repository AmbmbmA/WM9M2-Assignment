﻿#pragma once

#include "Mathlib.h"
#include "DXcore.h"
#include "Shaders.h"
#include "GEMLoader.h"
#include "Animation.h"
#include "Texture.h"


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

	void init(DXcore* core);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal) {

		Matrix Scaled = Matrix::Scaling(Scal);
		Matrix Scaledworld = (*World) * Scaled;

		shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
		shader->updateConstantVS("staticMeshBuffer", "VP", vp);
		shader->apply(core);

		mesh.draw(core);

	}


};

class Cube {
public:

	Mesh mesh;

	~Cube() { mesh.free(); }

	void init(DXcore* core);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal) {

		Matrix Scaled = Matrix::Scaling(Scal);
		Matrix Scaledworld = (*World) * Scaled;

		shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
		shader->updateConstantVS("staticMeshBuffer", "VP", vp);
		shader->apply(core);

		mesh.draw(core);

	}

};

class Sphere {
public:

	Mesh mesh;

	~Sphere() { mesh.free(); }

	void init(DXcore* core, int rings, int segments, float radius);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal) {

		Matrix Scaled = Matrix::Scaling(Scal);
		Matrix Scaledworld = (*World) * Scaled;

		shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
		shader->updateConstantVS("staticMeshBuffer", "VP", vp);
		shader->apply(core);

		mesh.draw(core);

	}


};




class StaticModel {
public:

	std::vector<Mesh> meshes;
	std::vector<std::string> textureFilenames;

	~StaticModel() {
		for (auto m : meshes) {
			m.free();
		}
	}

	void init(DXcore* core, string filename);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures);

	void drawt(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures) {

		Matrix Scaled = Matrix::Scaling(Scal);
		Matrix Scaledworld = (*World) * Scaled;

		shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
		shader->updateConstantVS("staticMeshBuffer", "VP", vp);
		shader->apply(core);

		string a = "Textures/rounded-brick1-albedo.png";

		for (int i = 0; i < meshes.size(); i++)
		{
			shader->bindShaderRV(core, "tex", textures->find(a));
			meshes[i].draw(core);
		}

	}

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

	void init(DXcore* core, string filename,int tilingnum);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures);

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

	void init(DXcore* core, string filename);

	void draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, AnimationInstance* instance, TextureManager* textures);

	void drawt(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, AnimationInstance* instance, TextureManager* textures) {

		Matrix Scaled = Matrix::Scaling(Scal);
		Matrix Scaledworld = (*World) * Scaled;

		shader->updateConstantVS("animatedMeshBuffer", "W", &Scaledworld);
		shader->updateConstantVS("animatedMeshBuffer", "VP", vp);
		shader->updateConstantVS("animatedMeshBuffer", "bones", instance->matrices);
		shader->apply(core);

		string a = "Textures/MaleDuty_3_OBJ_Happy_Packed0_Diffuse.png";

		for (int i = 0; i < meshes.size(); i++)
		{

			shader->bindShaderRV(core, "tex", textures->textures[a]->srv);
			meshes[i].draw(core);
		}

	}
};




