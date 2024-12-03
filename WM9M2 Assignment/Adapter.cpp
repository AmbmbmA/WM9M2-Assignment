#include"Adapter.h"

// so we can use NVIDIA GPU
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}


void Adapter::findadapter() {

	IDXGIAdapter1* adaptertemp; // temp for finding adapters
	std::vector<IDXGIAdapter1*> adaptersfound; // all adapters found

	// DXGI factory for enumerating
	IDXGIFactory6* factory = nullptr; 
	CreateDXGIFactory(__uuidof(IDXGIFactory6), (void**)&factory);

	// use factory method 1 to enumerate all
	int i = 0;
	while (factory->EnumAdapters1(i, &adaptertemp) != DXGI_ERROR_NOT_FOUND)
	{
		adaptersfound.push_back(adaptertemp); // all save in the vector
		i++;
	}

	// to find the adapter with max memory
	long long maxVideoMemory = 0;
	int useAdapterIndex = 0;
	// go through all adapters found
	for (int i = 0; i < adaptersfound.size(); i++)
	{
		DXGI_ADAPTER_DESC desc; // a struct stores info of adpater
		adaptersfound[i]->GetDesc(&desc); // load each adapters info
		if (desc.DedicatedVideoMemory > maxVideoMemory)
		{
			maxVideoMemory = desc.DedicatedVideoMemory;
			useAdapterIndex = i;
		}
	}

	// assign the adapter we need to use
	adapter = adaptersfound[useAdapterIndex];

	// free factory memory
	factory->Release();

}