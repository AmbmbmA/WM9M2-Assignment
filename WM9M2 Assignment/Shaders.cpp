#include "Shaders.h"


void Shader::init(string shadername, DXcore* core) {

	ID3DBlob* compiledVertexShader = LoadVertexShader(shadername);
	ID3DBlob* compiledPixelShader = LoadPixelShader(shadername);
	if (compiledVertexShader == nullptr) compiledVertexShader = ReadCompileSaveVS(shadername);
	if (compiledPixelShader == nullptr) compiledPixelShader = ReadCompileSavePS(shadername);

	CreateLayout(core, compiledVertexShader, shadername);

	CreateVS(core, compiledVertexShader);
	CreatePS(core, compiledPixelShader);

}

string Shader::readfile(string filename) {
	ifstream read;
	stringstream buffer;
	read.open(filename, ios::in);
	buffer << read.rdbuf();
	read.close();
	return buffer.str();
}

ID3DBlob* Shader::ReadCompileSaveVS(string shadername) {

	ID3DBlob* compiledVertexShader;

	// read
	string vertexshaderfilename = "Shaders/VS-" + shadername + ".txt";
	string vertexshaderstring = readfile(vertexshaderfilename);

	ID3DBlob* status;

	// compile 
	HRESULT hr = D3DCompile(
		vertexshaderstring.c_str(),  // source code in C style string
		strlen(vertexshaderstring.c_str()),  // size of the source code
		vertexshaderstring.c_str(), // code name (for error message)
		nullptr, nullptr, // macro and includes ( not used)
		"VS", // code entry point name
		"vs_5_0", // target profile
		0, 0,  // compile flags
		&compiledVertexShader, // compiled shader
		&status // error message
	);


	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)status->GetBufferPointer(), "VertexShaderError", 0);
		exit(0);
	}

	//save
	string vertexshadercompiledfilename = "Shaders/VS-" + shadername + "compiled.cso";
	wstring widename = wstring(vertexshadercompiledfilename.begin(), vertexshadercompiledfilename.end());

	hr = D3DWriteBlobToFile(compiledVertexShader, widename.c_str(), FALSE);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"SaveError", L"SaveError", MB_ICONERROR);
		exit(0);
	}

	return compiledVertexShader;
}

ID3DBlob* Shader::ReadCompileSavePS(string shadername) {

	ID3DBlob* compiledPixelShader;

	// read
	string pixelshaderfilename = "Shaders/PS-" + shadername + ".txt";
	string pixelshaderstring = readfile(pixelshaderfilename);

	ID3DBlob* status;

	// compile
	HRESULT hr = D3DCompile(
		pixelshaderstring.c_str(),
		strlen(pixelshaderstring.c_str()),
		pixelshaderstring.c_str(),
		nullptr, nullptr,
		"PS",
		"ps_5_0",
		0, 0,
		&compiledPixelShader,
		&status
	);


	if (FAILED(hr))
	{
		MessageBoxA(nullptr, (char*)status->GetBufferPointer(), "PixelShaderError", 0);
		exit(0);
	}

	// save
	string pixelshadercompiledfilename = "Shaders/PS-" + shadername + "compiled.cso";
	wstring widename = wstring(pixelshadercompiledfilename.begin(), pixelshadercompiledfilename.end());

	hr = D3DWriteBlobToFile(compiledPixelShader, widename.c_str(), FALSE);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"SaveError", L"SaveError", MB_ICONERROR);
		exit(0);
	}

	return compiledPixelShader;
}

ID3DBlob* Shader::LoadVertexShader(string shadername) {

	ID3DBlob* compiledVertexShader = nullptr;

	string vertexshadercompiledfilename = "Shaders/VS-" + shadername + "compiled.cso";
	wstring widename = wstring(vertexshadercompiledfilename.begin(), vertexshadercompiledfilename.end());

	D3DReadFileToBlob(widename.c_str(), &compiledVertexShader);

	return compiledVertexShader;
}

ID3DBlob* Shader::LoadPixelShader(string shadername) {

	ID3DBlob* compiledPixelShader = nullptr;

	string pixelshadercompiledfilename = "Shaders/PS-" + shadername + "compiled.cso";
	wstring widename = wstring(pixelshadercompiledfilename.begin(), pixelshadercompiledfilename.end());

	D3DReadFileToBlob(widename.c_str(), &compiledPixelShader);

	return compiledPixelShader;
}

void Shader::CreateLayout(DXcore* core, ID3DBlob* compiledVertexShader, string shadername) {


	// define layout
	D3D11_INPUT_ELEMENT_DESC layout_simple[] =
	{
		{ "POSITION", // Semantic name
		0, // if there is multi property for this name
		DXGI_FORMAT_R32G32B32_FLOAT, // data type (float3)
		0, // which vertex buffer
		D3D11_APPEND_ALIGNED_ELEMENT,  // auto offset
		D3D11_INPUT_PER_VERTEX_DATA, 0 // data for each vertex
		},
		{ "COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC layout_definedshape[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};

	D3D11_INPUT_ELEMENT_DESC layout_static[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};

	D3D11_INPUT_ELEMENT_DESC layout_animated[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

	};


	// choose the correct one
	D3D11_INPUT_ELEMENT_DESC* layoutDesc = nullptr;
	int num = 0;

	if (shadername == "simple") {
		layoutDesc = layout_simple;
		num = 2;
	}
	else if (shadername == "definedshape") {
		layoutDesc = layout_definedshape;
		num = 5;
	}
	else if (shadername == "static") {
		layoutDesc = layout_static;
		num = 5;
	}
	else if (shadername == "animated") {
		layoutDesc = layout_animated;
		num = 7;
	}
	else {
		MessageBox(nullptr, L"LayoutError", L"LayoutError", MB_ICONERROR);
		exit(0);
	}


	// create the layout
	core->device->CreateInputLayout(
		layoutDesc, 
		num, //
		compiledVertexShader->GetBufferPointer(), 
		compiledVertexShader->GetBufferSize(), 
		&layout // latout created
	);

}

void Shader::CreateVS(DXcore* core, ID3DBlob* compiledVertexShader) {

	core->device->CreateVertexShader(compiledVertexShader->GetBufferPointer(), compiledVertexShader->GetBufferSize(),nullptr, &vertexShader);

	ConstantBufferReflection reflection;
	reflection.build(core, compiledVertexShader, vsConstantBuffers, textureBindPointsVS, ShaderStage::VertexShader);
}

void Shader::CreatePS(DXcore* core, ID3DBlob* compiledPixelShader) {
	core->device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), nullptr, &pixelShader);

	ConstantBufferReflection reflection;
	reflection.build(core, compiledPixelShader, psConstantBuffers, textureBindPointsPS, ShaderStage::PixelShader);
}

void Shader::apply(DXcore* core) {
	
	// send to GPU
	core->devicecontext->IASetInputLayout(layout);
	core->devicecontext->VSSetShader(vertexShader, NULL, 0);
	core->devicecontext->PSSetShader(pixelShader, NULL, 0);


	for (int i = 0; i < vsConstantBuffers.size(); i++)
	{
		vsConstantBuffers[i].upload(core);
	}
	for (int i = 0; i < psConstantBuffers.size(); i++)
	{
		psConstantBuffers[i].upload(core);
	}

}