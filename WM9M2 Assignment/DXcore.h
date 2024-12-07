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

	static const int RENDERTARNUM = 3; // number of render targets
	ID3D11Texture2D* renderTargets[RENDERTARNUM];
	ID3D11RenderTargetView* renderTargetViews[RENDERTARNUM];
	ID3D11ShaderResourceView* shaderResourceViews[RENDERTARNUM];

	//ID3D11Texture2D* shadowTexture = nullptr; 
	//ID3D11DepthStencilView* shadowDSV = nullptr; 
	//ID3D11ShaderResourceView* shadowSRV = nullptr; 

	// for depth buffer and stencil buffer
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthbuffer;

	ID3D11RasterizerState* rasterizerState; // control rasterizer for culling etc.

	ID3D11DepthStencilState* depthStencilState;

	ID3D11BlendState* blendState;

	~DXcore() {
		if (device)device->Release();
		if (devicecontext)devicecontext->Release();
		if (swapchain)swapchain->Release();
		if (backbufferRenderTargetView)backbufferRenderTargetView->Release();
		if (backbuffer)backbuffer->Release();
		if (depthStencilView)depthStencilView->Release();
		if (backbuffer)backbuffer->Release();
		if (rasterizerState)rasterizerState->Release();
		if (depthStencilState)depthStencilState->Release();
		if (blendState)blendState->Release();
		//for (int i = 0; i < RENDERTARNUM; i++) {
		//	renderTargets[i]->Release();
		//	renderTargetViews[i]->Release();
		//	shaderResourceViews[i]->Release();
		//}
	}

	void init(int width, int height, HWND hwnd, bool window_fullscreen);

	void createRenderTargets(int width, int height);

	void settoGbuffer() {
		devicecontext->OMSetRenderTargets(RENDERTARNUM, renderTargetViews, depthStencilView);

	}

	void settoBackbuffer() {
		devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);

	}

	void settoshadowmap() {
		//devicecontext->OMSetRenderTargets(0, nullptr, shadowDSV); 
		//devicecontext->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); 

	}

	void clear();

	void present();


};