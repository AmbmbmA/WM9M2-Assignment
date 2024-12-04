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

Matrix StandardLocation(Vec3 position) {
	return Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), position);
}

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
	camera.init(Vec3(0, 2, 0), 90, 0, projection, &win);

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	// Static

	Shader shaderstatic;
	shaderstatic.init("static", &core);

	// basic shape
	Plane p;
	p.init(&core);
	Matrix planeWorld = StandardLocation(Vec3(0, 0, 0));

	Cube c;
	c.init(&core);
	Matrix cubeWorld = StandardLocation(Vec3(3, 5, 3));

	Sphere sphere;
	sphere.init(&core, 100, 100, 2);
	Matrix sphereWorld = StandardLocation(Vec3(3, 2, 3));

	StaticModel acacia;
	acacia.init(&core, "Resources/Models/acacia_003.gem");
	Matrix acaciaWorld = StandardLocation(Vec3(3, 0, -3));

	// Animated
	Shader shaderanimated;
	shaderanimated.init("animated", &core);

	AnimationInstance TRexins;
	AnimatedModel TRex;
	TRex.init(&core, "Resources/Models/TRex.gem");
	Vec3 TRexposition = Vec3(-10, 0, -10);
	Matrix TRexWorld = StandardLocation(TRexposition);

	TRexins.animation = &TRex.animation;
	TRexins.currentAnimation = "Run";


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


		camera.update(dt);

		p.draw(&core, &shaderstatic, &planeWorld, &camera.vp,Vec3(1,1,1));

		c.draw(&core, &shaderstatic, &cubeWorld, &camera.vp, Vec3(1, 1, 1));

		sphereWorld = StandardLocation(Vec3(3, 2, 3));

		sphere.draw(&core, &shaderstatic, &sphereWorld, &camera.vp, Vec3(1, 1, 1));

		Matrix sphereWorld = StandardLocation(Vec3(10, 2, 10));

		sphere.draw(&core, &shaderstatic, &sphereWorld, &camera.vp, Vec3(1, 1, 1));

		acacia.draw(&core, &shaderstatic, &acaciaWorld, &camera.vp, Vec3(0.01, 0.01, 0.01));


		

		if ((camera.position - TRexposition).getlength() >= 10) {
			TRexins.update("walk", dt);

		}
		else {
			TRexins.update("Run", dt);
		}
		TRex.draw(&core, &shaderanimated, &TRexWorld, &camera.vp, Vec3(1, 1, 1), &TRexins);


		core.present();
	}

}
