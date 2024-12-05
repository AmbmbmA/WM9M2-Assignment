#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	Sampler sampler;
	sampler.init(&core);


	// Camera
	Matrix projection = Matrix::Perspectiveprojectionz01(0.01, 100, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
	Camera camera;
	camera.init(Vec3(0, 3, 0), 90, 0, projection, &win);

	ShaderManager shaders;
	shaders.load(&core, "definedshape");
	shaders.load(&core, "static");
	shaders.load(&core, "animated");

	TextureManager textures;
	textures.load(&core, "stump01.png");
	textures.load(&core, "bark09.png");
	textures.load(&core, "pine branch.png");
	textures.load(&core, "rounded-brick1-albedo.png");
	textures.load(&core, "T-rex_Base_Color.png");
	textures.load(&core, "MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");

	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Gloss.png");
	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Normal.png");
	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Specular.png");

	Plane p;
	p.init(&core,10);
	Matrix planeWorld = StandardLocation(Vec3(0, -0.05, 0));

	Cube c;
	c.init(&core);
	Matrix cWorld = StandardLocation(Vec3(3, 5, 3));

	Sphere sphere;
	sphere.init(&core, 100, 100, 2);
	Matrix sphereWorld = StandardLocation(Vec3(3, 2, 3));


	// pine

	StaticModel pine;
	pine.init(&core, "Models/pine.gem");
	Matrix pineWorld = StandardLocation(Vec3(3, 0, -3));



	// cube

	StaticModelwithtiling cube;
	cube.init(&core, "Models/cube.gem",3);
	Matrix cubeWorld = StandardLocation(Vec3(0, -0.05, 0));


	// TRex
	AnimationInstance TRexins;
	AnimatedModel TRex;
	TRex.init(&core, "Models/TRex.gem");

	Vec3 TRexposition = Vec3(-10, 0, -10);
	Matrix TRexWorld = StandardLocation(TRexposition);

	TRexins.animation = &TRex.animation;
	TRexins.currentAnimation = "Run";



	// T

	AnimationInstance T;
	AnimatedModel TT;
	TT.init(&core, "Models/Soldier1.gem");

	Vec3 Tp = Vec3(-5, 0, -5);
	Matrix Tw = StandardLocation(Tp);

	T.animation = &TT.animation;
	T.currentAnimation = "idle aiming";



	while (run)
	{
		//win.clipMouseToWindow();

		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();
		win.processMessages();

		sampler.bind(&core);

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
		//if (win.keys['P']) {
		//	camera.switchview(true);
		//}
		//if (win.keys['L']) {
		//	camera.switchview(false);
		//}



		camera.update(dt);

		p.draw(&core, shaders.find("static"), &planeWorld, &camera.vp, Vec3(1, 1, 1),&textures);

		c.draw(&core, shaders.find("definedshape"), &cWorld, &camera.vp, Vec3(1, 1, 1));

		sphereWorld = StandardLocation(Vec3(3, 2, 3));

		sphere.draw(&core, shaders.find("definedshape"), &sphereWorld, &camera.vp, Vec3(1, 1, 1));

		pine.draw(&core, shaders.find("static"), &pineWorld, &camera.vp, Vec3(0.01, 0.01, 0.01), &textures);

		cubeWorld = StandardLocation(Vec3(0, -0.05, 0));

		cube.drawt(&core, shaders.find("static"), &cubeWorld, &camera.vp, Vec3(0.5, 0.5, 0.5), &textures);


		if ((camera.position - TRexposition).getlength() >= 10) {
			TRexins.update("walk", dt);

		}
		else {
			TRexins.update("Run", dt);
		}
		TRex.draw(&core, shaders.find("animated"), &TRexWorld, &camera.vp, Vec3(1, 1, 1), &TRexins, &textures);



		T.update("idle aiming", dt);

		TT.draw(&core, shaders.find("animated"), &Tw, &camera.vp, Vec3(0.02, 0.02, 0.02), &T, &textures);


		core.present();
	}

}
