
#include "Mesh.h"

using namespace Mathlib;


void Mesh::init(DXcore* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {

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
	indicesSize = numIndices;
	strides = vertexSizeInBytes;

}

void Triangle::init(DXcore* core, int N) {

	// triangle vertex data
	vertices[0].position = Vec3(0, 1.0f, 0.5);
	vertices[0].colour = Colour(0, 1.0f, 0);
	vertices[1].position = Vec3(-1.0f, -1.0f, 0.5);
	vertices[1].colour = Colour(1.0f, 0, 0);
	vertices[2].position = Vec3(1.0f, -1.0f, 0.5);
	vertices[2].colour = Colour(0, 0, 1.0f);


	// vertex buffer info set
	D3D11_BUFFER_DESC vbinfo;
	vbinfo.Usage = D3D11_USAGE_DEFAULT; // default setting for read and write by GPU
	vbinfo.CPUAccessFlags = 0; // CPU is not allowed to access directly
	vbinfo.MiscFlags = 0;
	vbinfo.ByteWidth = sizeof(Vertex) * N; // size of the buffer 
	vbinfo.BindFlags = D3D11_BIND_VERTEX_BUFFER; // specify this as the vertex buffer

	// initial data
	D3D11_SUBRESOURCE_DATA uploadData;
	uploadData.pSysMem = vertices;
	uploadData.SysMemPitch = 0;
	uploadData.SysMemSlicePitch = 0;

	// creat the vertex buffer
	core->device->CreateBuffer(&vbinfo, &uploadData, &vertexBuffer);

}

void Triangle::draw(DXcore* core) {

	// offset value of the vertices
	UINT offsets;
	offsets = 0;

	// size of the info of each vertex
	UINT strides = sizeof(Vertex);

	// set topology method using triangle list
	core->devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind the vertex buffer
	core->devicecontext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);

	//draw 3 for vertex num, 0 for starting vertex index
	core->devicecontext->Draw(3, 0);

}