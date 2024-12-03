#include "DXcore.h"


void DXcore::init(int width, int height, HWND hwnd, bool window_fullscreen) {

	DXGI_SWAP_CHAIN_DESC scinfo; // a struct for swap chain info
	memset(&scinfo, 0, sizeof(scinfo)); // initialize swapchain info as 0
	{
		scinfo.BufferCount = 1; // has one backbuffer ( double buffering)

		// size of the buffer
		scinfo.BufferDesc.Width = width;
		scinfo.BufferDesc.Height = height;

		scinfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // pixel format ( 8bit rgba)

		// refresh rate (60fps)
		scinfo.BufferDesc.RefreshRate.Numerator = 60;
		scinfo.BufferDesc.RefreshRate.Denominator = 1;

		scinfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // use for buffer ( for reder output)
		scinfo.OutputWindow = hwnd; // handle of the window to show

		//multisample ( set 1 to not use)
		scinfo.SampleDesc.Count = 1;
		scinfo.SampleDesc.Quality = 0;

		scinfo.Windowed = window_fullscreen ? false : true; // full screen or not
		scinfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // flag set as allow changing between window and fullscreen

	}

	// claim the DirectX version as DirectX11
	D3D_FEATURE_LEVEL DXversion;
	DXversion = D3D_FEATURE_LEVEL_11_0;

	// find the adapter we want
	Adapter adapter;
	adapter.findadapter();

	// create device, swap chain, device context
	D3D11CreateDeviceAndSwapChain(
		adapter.adapter, // spcigy a GPU adapter to create device
		D3D_DRIVER_TYPE_UNKNOWN, // drive type unknown ( need to specify the adpater above)
		nullptr, // if drive type is set as software, specify here, usually nullptr
		D3D11_CREATE_DEVICE_DEBUG, // let DirectX offer more debugging info
		&DXversion, // which version the device is
		1, // only one version
		D3D11_SDK_VERSION, // Direct3D SDK version
		&scinfo, // swap chain info
		&swapchain, // the swap chain created
		&device, // the device created
		nullptr, // to return the actual version used to created the device
		&devicecontext); // the device context created

	swapchain->SetFullscreenState(window_fullscreen,NULL); // set if full screen

	// get buffer from the swapchain
	swapchain->GetBuffer(
		0, 	// 0 is for back buffer
		__uuidof(ID3D11Texture2D),  // type of the buffer
		(LPVOID*)&backbuffer); // where to store the buffer get

	// use back buffer to create a render target view (default setting)
	device->CreateRenderTargetView(backbuffer, NULL, &backbufferRenderTargetView);

	// depth stencil buffer info
	D3D11_TEXTURE2D_DESC dsbinfo; // a struct for 2d texture
	{
		// size ( usually same as the window)
		dsbinfo.Width = width;
		dsbinfo.Height = height;

		dsbinfo.MipLevels = 1;
		dsbinfo.ArraySize = 1;

		dsbinfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // format for depth and stencil buffer

		//multisample ( set 1 to not use)
		dsbinfo.SampleDesc.Count = 1;
		dsbinfo.SampleDesc.Quality = 0;

		dsbinfo.Usage = D3D11_USAGE_DEFAULT;
		dsbinfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dsbinfo.CPUAccessFlags = 0;
		dsbinfo.MiscFlags = 0;
	}

	// creat depth stencil buffer
	device->CreateTexture2D(&dsbinfo, NULL, &depthbuffer);

	// use the depth stencil buffer to create a depth stencil view
	device->CreateDepthStencilView(depthbuffer, NULL, &depthStencilView);

	// send bbrv and sdv to GPU for rendering
	devicecontext->OMSetRenderTargets(1, &backbufferRenderTargetView, depthStencilView);

	// creat viewport and set
	D3D11_VIEWPORT viewport;
	{
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		// set the viewport at rasterizer stage
		devicecontext->RSSetViewports(1, &viewport); // 1 as only one viewport
	}

	// create rasterizer state and set
	D3D11_RASTERIZER_DESC rsinfo;
	ZeroMemory(&rsinfo, sizeof(rsinfo)); // initialize

	rsinfo.FillMode = D3D11_FILL_SOLID; // solid fill for shapes
	rsinfo.CullMode = D3D11_CULL_NONE; // cull nothing ( could back or front)

	// creat and pass to GPU
	device->CreateRasterizerState(&rsinfo, &rasterizerState);
	devicecontext->RSSetState(rasterizerState);

	// create depth and stencil state and set
	D3D11_DEPTH_STENCIL_DESC dssinfo;
	ZeroMemory(&dssinfo, sizeof(dssinfo));

	dssinfo.DepthEnable = true; // enable depth test             
	dssinfo.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;// allow read and write  
	dssinfo.DepthFunc = D3D11_COMPARISON_LESS; // closest to draw

	dssinfo.StencilEnable = false; // no stencil test      

	device->CreateDepthStencilState(&dssinfo, &depthStencilState);
	devicecontext->OMSetDepthStencilState(depthStencilState, 0);


	// blending state
	D3D11_BLEND_DESC bsinfo;
	ZeroMemory(&bsinfo, sizeof(bsinfo));

	bsinfo.AlphaToCoverageEnable = false;
	bsinfo.IndependentBlendEnable = false; // independent setting

	// target 0 alpha blending
	bsinfo.RenderTarget[0].BlendEnable = true;
	bsinfo.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsinfo.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsinfo.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bsinfo.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bsinfo.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bsinfo.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bsinfo.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&bsinfo, &blendState);
	devicecontext->OMSetBlendState(blendState, nullptr, 0xffffffff);

}

void DXcore::clear() {
	float ClearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // clear back ground 

	// clear the back buffer use set color
	devicecontext->ClearRenderTargetView(backbufferRenderTargetView, ClearColour); 

	// clear depth stencil view, depth reset to 1 (furthest), stencil reset to 0
	devicecontext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void DXcore::present() {

	// present ( swap chains etc.)
	swapchain->Present(
		0,  // decide use of Sync ( 1 to use)
		0 // extra flags
	);
}