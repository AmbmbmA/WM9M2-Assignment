
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
