#include "Window.h"
#include <memory>


Window* window;

// get mouse position
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

//for window messages (keyboard input, mousemovement,window close)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// msg( type of the message)
	// WPARAM (extra message information: e.g. which key is pressed)
	// LPARAM (extra message information: e.g. position of the mouse)
	switch (msg)
	{
	case WM_CLOSE: // request close of the window
	{
		// double check the request using message box
		//(HWND, text content, tile of the box, type of the box)
		
		//int result = MessageBox(hwnd, L"Confirm to close the Window", L"Confirmation", MB_OKCANCEL | MB_ICONQUESTION);
		//if (result == IDOK) {
		//	PostQuitMessage(0);
		//	exit(0);
		//	return 0;
		//}
		//else {
		//	return 0;
		//}

		PostQuitMessage(0); // quit message loop
		exit(0); // end the program
		return 0;
	}
	case WM_DESTROY: //actual close of the window
	{
		PostQuitMessage(0); // quit message loop
		exit(0); // end the program
		return 0;
	}
	case WM_KEYDOWN: // key is pressed
	{
		window->keys[(unsigned int)wParam] = true; // set the key state as true
		return 0;
	}
	case WM_KEYUP: //key is unpressed
	{
		window->keys[(unsigned int)wParam] = false;
		return 0;
	}
	case WM_LBUTTONDOWN: // left mouse button
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam)); //update mouse position
		window->mouseButtons[0] = true; // set mouse state as true
		return 0;
	}
	case WM_LBUTTONUP:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[0] = false;
		return 0;
	}
	case WM_RBUTTONDOWN: // left mouse button
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam)); //update mouse position
		window->mouseButtons[1] = true; // set mouse state as true
		return 0;
	}
	case WM_RBUTTONUP:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[1] = false;
		return 0;
	}
	case WM_MOUSEMOVE: // mouse move
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_INPUT: // raw input
	{
		UINT inputsize = 0;
		// transfer the info for raw input
		HRAWINPUT transferlParam = (HRAWINPUT)lParam;

		// get all input info ( if only want the device info, could use RID_HEADER)
		// pass the data into nullptr, only get the input size
		// pass the header size to read the info
		GetRawInputData(transferlParam, RID_INPUT, nullptr, &inputsize, sizeof(RAWINPUTHEADER));

		// if there is input
		if (inputsize > 0) {
			// a pointer to byte
			LPBYTE inputinfo = new BYTE[inputsize]; // set a memory to read the input

			// read into the memory 
			GetRawInputData(transferlParam, RID_INPUT, inputinfo, &inputsize, sizeof(RAWINPUTHEADER));
			
			// transfer from byte into RAWINPUT struct
			RAWINPUT* raw = (RAWINPUT*)inputinfo;

			// check if this is mouse input
			if (raw->header.dwType == RIM_TYPEMOUSE) {
				window->rawmousex = raw->data.mouse.lLastX; // pixel moved since last check, + right, - left
				window->rawmousey = raw->data.mouse.lLastY; // + up, -  down
			}

			delete[] inputinfo; // free

		}
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}

}

void Window::init(std::string window_name, int window_width, int window_height, int window_x, int window_y) {

	WNDCLASSEX wc; // a struct for properties of the window

	hinstance = GetModuleHandle(NULL); // let the handle be this program
	name = window_name;
	width = window_width;
	height = window_height;
	style = WS_VISIBLE | WS_OVERLAPPEDWINDOW /*& ~WS_THICKFRAME*/; // window style 
	memset(keys, 0, 256 * sizeof(char)); // initialize the key function
	window = this; // set this pointer to global to be used by WndProc

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; // set the callback function
	wc.cbClsExtra = 0; // extra memory for the window class
	wc.cbWndExtra = 0; // extra memory for each window
	wc.hInstance = hinstance; // set the handle of the program
	//wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // load the big icon as default
	wc.hIcon = (HICON)LoadImage(NULL, L"Icons/Game_icon_Cindy.ico", IMAGE_ICON, 512, 512, LR_LOADFROMFILE); // load the icon from file
	wc.hIconSm = wc.hIcon;  // load the small icon same as big
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // load cursor from default
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // back ground set as black
	wc.lpszMenuName = NULL; // no menu
	std::wstring wname = std::wstring(name.begin(), name.end()); // change name type from string to wide string for Windows API to use
	wc.lpszClassName = wname.c_str(); // set the window class name for later reference (using C style for Windows API to use)
	wc.cbSize = sizeof(WNDCLASSEX); // set the size of the class
	RegisterClassEx(&wc); // register window class for later use

	// create the window, retrun the handle of the window if success
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style, window_x, window_y, width, height, NULL, NULL, hinstance, this);
	// extension style: force to show at the bar
	// parent handle = nullptr
	// menu handle = nullptr
	// extra info pass this

	// register a device for raw input
	RAWINPUTDEVICE rawinputdevice;

	rawinputdevice.usUsagePage = 0x01; // set the type as desktop controls
	rawinputdevice.usUsage = 0x02; // set as mouse
	rawinputdevice.dwFlags = 0; // set as default
	rawinputdevice.hwndTarget = nullptr; // detect for all windows

	// device,number to register(use array for multi),size 
	RegisterRawInputDevices(&rawinputdevice, 1, sizeof(rawinputdevice));

}

void Window::processMessages() {
	// reset for rawinput
	rawmousex = 0;
	rawmousey = 0;

	MSG msg; // a strut to store message
	ZeroMemory(&msg, sizeof(MSG)); // empty the memory before use

	// message loop untill empty, won't block
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {// save into msg, without specify the window
		// 0,0 means all message is detected,
		// remove after detecting so once for each message

		TranslateMessage(&msg); // translate virtual key input into char input for keyboard input

		DispatchMessage(&msg); // send message to the defined WndProc, use the handle to find the window
	}

}




