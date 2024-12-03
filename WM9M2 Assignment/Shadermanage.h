#pragma once


#include "DXcore.h"
#include "Shaders.h"
#include <map>

using namespace std;

class ShaderManage {
	DXcore* core;
	map<string, Shader*> shadersmap;

	ShaderManage(DXcore* core) : core(core){}

	~ShaderManage() {
		for (auto& s : shadersmap) {
			delete s.second;
		}
		shadersmap.clear();
	}


};

