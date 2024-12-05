#pragma once

#include"DXcore.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include "ShaderReflection.h"

#include <d3dcompiler.h> // to compile HLSL
#include <d3d11shader.h> // get shaders info

#pragma comment(lib,"dxguid.lib")

using namespace std;

class Shader {
public:

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;


	// constant buffer 
	vector<ConstantBuffer> vsConstantBuffers;
	vector<ConstantBuffer> psConstantBuffers;

	map<string, int> textureBindPointsVS;
	map<string, int> textureBindPointsPS;

	void free() {
		if (vertexShader)vertexShader->Release();
		if (pixelShader)pixelShader->Release();
		if (layout)layout->Release();
	}

	void init(string shadername, DXcore* core);

	string readfile(string filename);

	ID3DBlob* ReadCompileSaveVS(string shadername);

	ID3DBlob* ReadCompileSavePS(string shadername);

	ID3DBlob* LoadVertexShader(string shadername);

	ID3DBlob* LoadPixelShader(string shadername);

	void CreateLayout(DXcore* core, ID3DBlob* compiledVertexShader, string layoutkey);

	void CreateVS(DXcore* core, ID3DBlob* compiledVertexShader);

	void CreatePS(DXcore* core, ID3DBlob* compiledPixelShader);

	void apply(DXcore*);

	void updateConstant(std::string constantBufferName, std::string variableName, void* data, std::vector<ConstantBuffer>& buffers)
	{
		for (int i = 0; i < buffers.size(); i++)
		{
			if (buffers[i].name == constantBufferName)
			{
				buffers[i].update(variableName, data);
				return;
			}
		}
	}

	void updateConstantVS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, vsConstantBuffers);
	}

	void updateConstantPS(std::string constantBufferName, std::string variableName, void* data)
	{
		updateConstant(constantBufferName, variableName, data, psConstantBuffers);
	}

	void bindShaderRV(DXcore* core, string name, ID3D11ShaderResourceView* srv) {
		core->devicecontext->PSSetShaderResources(textureBindPointsPS[name], 1, &srv);
	}

};


class ShaderManager
{
public:

	std::map<std::string, Shader*> Shaders;

	void load(DXcore* core, std::string shadername)
	{

		string filename = "Shaders/" + shadername;

		std::map<std::string, Shader*>::iterator it = Shaders.find(shadername);
		if (it != Shaders.end())
		{
			return;
		}
		Shader* shader = new Shader();

		shader->init(shadername, core);

		Shaders.insert({ shadername, shader });
	}

	Shader* find(std::string shadername)
	{
		return Shaders[shadername];
	}

	void unload(std::string shadername)
	{
		Shaders[shadername]->free();
		Shaders.erase(shadername);
	}

	~ShaderManager()
	{
		for (auto it = Shaders.cbegin(); it != Shaders.cend(); )
		{
			it->second->free();
			Shaders.erase(it++);
		}
	}

};