#pragma once

#include "Adapter.h"
#include "memory"

// device, context(command queue),swapchain
class DXcore {
public:
	ID3D11Device* device; // device
	ID3D11DeviceContext* devicecontext; //device content (command queue)
	IDXGISwapChain* swapchain; // swapchain

	ID3D11RenderTargetView* backbufferRenderTargetView; // target to be rander on back buffer
	ID3D11Texture2D* backbuffer; // back buffer

	// for depth buffer and stencil buffer
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;

	ID3D11RasterizerState* rasterizerState; // control rasterizer for culling etc.

	ID3D11DepthStencilState* depthStencilState; 

	ID3D11BlendState* blendState;

	void init(int width, int height, HWND hwnd, bool window_fullscreen);

	void clear();

	void present();


};