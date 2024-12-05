
#include "Mesh.h"

using namespace Mathlib;


void Mesh::init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices, vector<Vec3> instanceData,int _instancenum) {

	instancenum = _instancenum;
	indicesSize = numIndices;

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = indices;

	core->device->CreateBuffer(&bd, &data, &indexBuffer);

	bd.ByteWidth = vertexSizeInBytes * numVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertices;
	core->device->CreateBuffer(&bd, &data, &vertexBuffer);

	//for (int i = 0; i < instancenum; i++) {
	//}

	// instance
	bd.ByteWidth = sizeof(instanceData);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = instanceData.data();
	core->device->CreateBuffer(&bd, &data, &instanceBuffer);



	strides[0] = vertexSizeInBytes;
	strides[1] = sizeof(Vec3);

	buffers[0] = vertexBuffer;
	buffers[1] = instanceBuffer;

}


void Mesh::draw(DXcore* core) {

	unsigned int offsets[2] = { 0,0 };

	core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core->devicecontext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	//core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	core->devicecontext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//core->devicecontext->DrawIndexed(indicesSize, 0, 0);
	core->devicecontext->DrawIndexedInstanced(indicesSize, instancenum,0,0,0);

}


void Plane::init(DXcore* core,int tiling) {

	std::vector<STATIC_VERTEX> vertices;
	vertices.push_back(addVertexwithtiling(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0, tiling));
	vertices.push_back(addVertexwithtiling(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0, tiling));
	vertices.push_back(addVertexwithtiling(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1, tiling));
	vertices.push_back(addVertexwithtiling(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1, tiling));

	std::vector<unsigned int> indices;
	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(2); indices.push_back(3);

	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));

	mesh.init(core, vertices, indices, instanceData,1);

}


void Cube::init(DXcore* core) {

	std::vector<STATIC_VERTEX> vertices;
	Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
	Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
	Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
	Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
	Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
	Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
	Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
	Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

	vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));

	vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
	vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
	vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
	vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

	std::vector<unsigned int> indices;
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(4); indices.push_back(6); indices.push_back(7);
	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(8); indices.push_back(10); indices.push_back(11);
	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(12); indices.push_back(14); indices.push_back(15);
	indices.push_back(16); indices.push_back(17); indices.push_back(18);
	indices.push_back(16); indices.push_back(18); indices.push_back(19);
	indices.push_back(20); indices.push_back(21); indices.push_back(22);
	indices.push_back(20); indices.push_back(22); indices.push_back(23);

	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));

	mesh.init(core, vertices, indices, instanceData, 1);
}


void Sphere::init(DXcore* core, int rings, int segments, float radius) {

	std::vector<STATIC_VERTEX> vertices;
	for (int lat = 0; lat <= rings; lat++) {
		float theta = lat * M_PI / rings;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		for (int lon = 0; lon <= segments; lon++) {
			float phi = lon * 2.0f * M_PI / segments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);
			Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta, radius * sinTheta * sinPhi);
			Vec3 normal = position.normalize();
			float tu = 1.0f - (float)lon / segments;
			float tv = 1.0f - (float)lat / rings;
			vertices.push_back(addVertex(position, normal, tu, tv));
		}
	}

	std::vector<unsigned int> indices;
	for (int lat = 0; lat < rings; lat++)
	{
		for (int lon = 0; lon < segments; lon++)
		{
			int current = lat * (segments + 1) + lon;
			int next = current + segments + 1;
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}


	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));
	instanceData.push_back(Vec3(3, 0, 3));

	mesh.init(core, vertices, indices, instanceData, 2);


}


void StaticModel::init(DXcore* core, string filename) {

	GEMLoader::GEMModelLoader loader;

	std::vector<GEMLoader::GEMMesh> gemmeshes;

	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));

	loader.load(filename, gemmeshes);

	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			vertices.push_back(v);
		}
		textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
		mesh.init(core, vertices, gemmeshes[i].indices, instanceData,1);
		meshes.push_back(mesh);
	}
}


void StaticModel::draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures) {

	Matrix Scaled = Matrix::Scaling(Scal);
	Matrix Scaledworld = (*World) * Scaled;

	shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
	shader->updateConstantVS("staticMeshBuffer", "VP", vp);
	shader->apply(core);


	for (int i = 0; i < meshes.size(); i++)
	{
		shader->bindShaderRV(core, "tex", textures->find(textureFilenames[i]));
		meshes[i].draw(core);
	}

}


void StaticModelwithtiling::init(DXcore* core, string filename, int tilingnum) {

	GEMLoader::GEMModelLoader loader;

	std::vector<GEMLoader::GEMMesh> gemmeshes;

	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));

	loader.load(filename, gemmeshes);

	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<STATIC_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
			STATIC_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
			v.tu *= tilingnum;
			v.tv *= tilingnum;
			vertices.push_back(v);
		}
		textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
		mesh.init(core, vertices, gemmeshes[i].indices, instanceData,1);
		meshes.push_back(mesh);
	}
}


void StaticModelwithtiling::draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, TextureManager* textures) {

	Matrix Scaled = Matrix::Scaling(Scal);
	Matrix Scaledworld = (*World) * Scaled;

	shader->updateConstantVS("staticMeshBuffer", "W", &Scaledworld);
	shader->updateConstantVS("staticMeshBuffer", "VP", vp);
	shader->apply(core);


	for (int i = 0; i < meshes.size(); i++)
	{
		shader->bindShaderRV(core, "tex", textures->find(textureFilenames[i]));
		meshes[i].draw(core);
	}

}


void AnimatedModel::init(DXcore* core, string filename) {

	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	GEMLoader::GEMAnimation gemanimation;

	vector<Vec3> instanceData;
	instanceData.push_back(Vec3(0, 0, 0));

	loader.load(filename, gemmeshes, gemanimation);

	listAnimationNames(gemanimation);

	for (int i = 0; i < gemmeshes.size(); i++) {
		Mesh mesh;
		std::vector<ANIMATED_VERTEX> vertices;
		for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
			ANIMATED_VERTEX v;
			memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
			vertices.push_back(v);
		}
		textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
		mesh.init(core, vertices, gemmeshes[i].indices, instanceData,1);
		meshes.push_back(mesh);
	}

	for (int i = 0; i < gemanimation.bones.size(); i++)
	{
		Bone bone;
		bone.name = gemanimation.bones[i].name;
		memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
		bone.parentIndex = gemanimation.bones[i].parentIndex;
		animation.skeleton.bones.push_back(bone);
	}

	for (int i = 0; i < gemanimation.animations.size(); i++)
	{
		std::string name = gemanimation.animations[i].name;
		AnimationSequence aseq;
		aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
		for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
		{
			AnimationFrame frame;
			for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
			{
				Vec3 p;
				Quaternion q;
				Vec3 s;
				memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
				frame.positions.push_back(p);
				memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
				frame.rotations.push_back(q);
				memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
				frame.scales.push_back(s);
			}
			aseq.frames.push_back(frame);
		}
		animation.animations.insert({ name, aseq });
	}

}

void AnimatedModel::draw(DXcore* core, Shader* shader, Matrix* World, Matrix* vp, Vec3 Scal, AnimationInstance* instance, TextureManager* textures) {

	Matrix Scaled = Matrix::Scaling(Scal);
	Matrix Scaledworld = (*World) * Scaled;

	shader->updateConstantVS("animatedMeshBuffer", "W", &Scaledworld);
	shader->updateConstantVS("animatedMeshBuffer", "VP", vp);
	shader->updateConstantVS("animatedMeshBuffer", "bones", instance->matrices);
	shader->apply(core);

	for (int i = 0; i < meshes.size(); i++)
	{
		shader->bindShaderRV(core, "tex", textures->find(textureFilenames[i]));
		meshes[i].draw(core);
	}

}