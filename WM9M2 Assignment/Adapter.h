#pragma once

#include <D3D11.h> // directX11 API
#include <vector>
#include <dxgi1_6.h> //DXGI(DirectX Graphics Infrastructure), to manage the graphic cards

#pragma comment(lib, "D3D11.lib") // lib of functions of DirectX11
#pragma comment(lib, "D3DCompiler.lib") // compiler for HLSL
#pragma comment(lib, "DXGI.lib") //DXGI manage

class Adapter {
public:
	// poiter to DXGI adapter (GPU)
	IDXGIAdapter1* adapter; 

	void findadapter();

};