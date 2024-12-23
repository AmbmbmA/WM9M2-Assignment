#pragma once

#include <Windows.h>
#include <string>


class Window {
public:
	HWND hwnd; // handle that specify which window is receiving message
	HINSTANCE hinstance; // handle that specify memory of the program

	std::string name; // window name
	int width;
	int	height;
	DWORD style; // windoe style 

	bool keys[256]; // track key press
	int mousex; // mouse position
	int mousey;
	bool mouseButtons[3]; // track mouse press

	float rawmousex = 0;
	float rawmousey = 0;

	void init(std::string window_name, int window_width, int window_height, int window_x = 0, int window_y = 0); //initialize the window

	void processMessages();

	// update mouseposition
	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	}

	// hide mouse
	bool hidec = false;

	void hideCursor() {
		if (!hidec) {
			ShowCursor(false);
			hidec = true;
		}
	}

	void showCursor() {
		if (hidec) {
			ShowCursor(true);
			hidec = false;
		}
	}

	void clipMouseToWindow();
};



