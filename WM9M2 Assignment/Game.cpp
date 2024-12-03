#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"

//int screenWidth = GetSystemMetrics(SM_CXSCREEN);
//int screenHeight = GetSystemMetrics(SM_CYSCREEN);

const int WINDOWSIZE[2] = { 1920,1080 };
const float FOV = 90;

// Windows program entrance
// prameters are set by the windows system
int WinMain(
	HINSTANCE hInstance, // handle specify the program
	HINSTANCE hPrevInstance, // handle used for specify if there is other program running , only for x16 Windows, now is NULL
	PSTR lpCmdLine, //  for command lines to pass extra info
	int nCmdShow // for initial window setting
) {

	GamesEngineeringBase::Timer timer;
	float time = 0.0f;

	bool run = true;


	Window win;
	win.init("Cindy's Adventure", WINDOWSIZE[0], WINDOWSIZE[1]);

	Matrix projection = Matrix::Perspectiveprojectionz01(1, 100, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
	Camera camera;
	camera.init(Vec3(0, 0, 0), 90, 0, projection, &win);

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	Plane p;
	p.init(&core);
	Matrix planeWorld = Matrix::Transformationto(Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1), Vec3(0, -2, 0));

	Cube c;
	c.init(&core);
	Matrix cubeWorld = Matrix::Transformationto(Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1), Vec3(3, 10, 3));


	Shader shaderstatic;
	shaderstatic.init("static", &core);
	shaderstatic.apply(&core);

	while (run)
	{
		//win.clipMouseToWindow();

		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();
		win.processMessages();



		if (win.keys['H']) {
			win.hideCursor();
		}
		if (win.keys['Y']) {
			win.showCursor();
		}
		//if (win.rawmousex != 0 || win.rawmousey != 0) {
		//	win.windowmove(win.rawmousex, win.rawmousey);
		//}

		//quit game
		if (win.keys[VK_ESCAPE]) {
			PostMessage(win.hwnd, WM_CLOSE, 0, 0); //post a message to requst closing the window without blocking
			win.keys[VK_ESCAPE] = false;
		}

		//camera.changetoSet(1, time);

		camera.update();

		p.mesh.draw(&core, &shaderstatic, &planeWorld, &camera.vp);

		c.mesh.draw(&core, &shaderstatic, &cubeWorld, &camera.vp);

		core.present();
	}

}
