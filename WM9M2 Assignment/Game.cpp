#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"

const int WINDOWSIZE[2] = { 1024, 720 };
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

	Camera camera;
	camera.init(Vec3(10, 5, 10), Vec3(0, 0, 0), Vec3(0, 1, 0));

	Matrix view, vp;
	Vec3 from, to;

	Matrix planeWorld = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 10, 0));

	Matrix cubeWorld = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));

	Matrix projection = Matrix::Perspectiveprojectionz01(1, 100, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);

	Window win;
	win.init("Cindy's Adventure", WINDOWSIZE[0], WINDOWSIZE[1]);
	bool run = true;

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	Plane p;
	p.init(&core);

	Cube c;
	c.init(&core);


	Shader shaderstatic;
	shaderstatic.init("static", &core);
	shaderstatic.apply(&core);


	float camerax = 0;
	float cameraz = 0;

	while (run)
	{
		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();
		win.processMessages();

		// key press


		if (win.keys['W']) {
			cameraz += u;
		}
		if (win.keys['S']) {
			cameraz -= u;
		}
		if (win.keys['A']) {
			camerax -= u;
		}
		if (win.keys['D']) {
			camerax += u;
		}



		//if (win.keys['Z']) {
		//	win.hideCursor();
		//}
		//if (win.keys['X']) {
		//	win.showCursor();
		//}
		//if (win.rawmousex != 0 || win.rawmousey != 0) {
		//	win.windowmove(win.rawmousex, win.rawmousey);
		//}
		if (win.keys[VK_ESCAPE]) {
			PostMessage(win.hwnd, WM_CLOSE, 0, 0); //post a message to requst closing the window without blocking
			win.keys[VK_ESCAPE] = false;
		}


		//from = Vec3(10 * cosf(time), 5, 10 * sinf(time));
		//to = Vec3(0, 0, 0);
		//view = view.LookAt(from, to, Vec3(0, 1, 0));

		camera.changetoSet(1, time);

		vp = projection * camera.view;

		p.mesh.draw(&core, &shaderstatic, &planeWorld, &vp);

		shaderstatic.updateConstantVS("staticMeshBuffer", "W", &cubeWorld);

		c.mesh.draw(&core, &shaderstatic, &cubeWorld, &vp);




		core.present();
	}

}
