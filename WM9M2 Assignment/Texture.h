#pragma once

#include "stb_image.h"
#include "DXcore.h"
#include <string>
#include <map>

using namespace std;

class Texture {
public:

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	void free() {
		texture->Release();
		srv->Release();
	}

	void init(DXcore* core, int width, int height, int channels, DXGI_FORMAT format, void* data);

	void load(DXcore* core, string filename);

};

class Sampler {
public:

	ID3D11SamplerState* state;

	~Sampler() {
		state->Release();
	}

	void init(DXcore* core);

	void bind(DXcore* core) {
		core->devicecontext->PSSetSamplers(0, 1, &state);
	}

};

class TextureManager
{
public:

	std::map<std::string, Texture*> textures;

	~TextureManager()
	{
		for (auto it = textures.cbegin(); it != textures.cend(); )
		{
			it->second->free();
			textures.erase(it++);
		}
	}

	void load(DXcore* core, std::string texturefilename);

	ID3D11ShaderResourceView* find(std::string name)
	{
		return textures[name]->srv;
	}

	void unload(std::string name)
	{
		textures[name]->free();
		textures.erase(name);
	}



};