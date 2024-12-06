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

	Matrix W = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));

	// Camera
	Matrix projection = Matrix::Perspectiveprojectionz01(0.01, 10000, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
	Camera camera;
	camera.init(Vec3(0, 3, 0), 90, 0, projection, &win);

	ShaderManager shaders;
	//shaders.load(&core, "definedshape");
	//shaders.load(&core, "static");
	//shaders.load(&core, "animated");
	shaders.load(&core, "definedshapeG");
	shaders.load(&core, "definedshapeL");


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
	vector<Vec3> pinslocation;
	pinslocation.push_back(Vec3(0, 0, 0));
	p.init(&core, 10, pinslocation, 1);

	Cube c;
	vector<Vec3> cinslocation;
	cinslocation.push_back(Vec3(3, 5, 3));
	c.init(&core, cinslocation, 1);

	Sphere sphere;
	vector<Vec3> sphereinslocation;
	sphereinslocation.push_back(Vec3(10, 2, 10));
	sphereinslocation.push_back(Vec3(10, 5, 10));
	sphere.init(&core, 100, 100, 2, sphereinslocation, 2);


	// pine

	StaticModel pine;
	vector<Vec3> pineinslocation;
	pineinslocation.push_back(Vec3(-5, 0, -5));
	pine.init(&core, "Models/pine.gem", pineinslocation, 1);



	// cube

	StaticModelwithtiling cube;
	vector<Vec3> cubeinslocation;
	cubeinslocation.push_back(Vec3(-5, 7, -5));
	cube.init(&core, "Models/cube.gem", 3, cubeinslocation, 1);


	// TRex

	AnimatedModel TRex;
	vector<Vec3> TRexinslocation;
	TRexinslocation.push_back(Vec3(10,0,10));
	TRexinslocation.push_back(Vec3(5,0,5));
	TRexinslocation.push_back(Vec3(20,0,20));
	TRex.init(&core, "Models/TRex.gem", TRexinslocation, 3);

	AnimationInstance TRexins;
	TRexins.animation = &TRex.animation;
	TRexins.currentAnimation = "Run";


	AnimatedModel Soldier;
	vector<Vec3> Soldierinslocation;
	Soldierinslocation.push_back(Vec3(0.0f, 0.0f, 0.0f));
	//Soldierinslocation.push_back(Vec3(15, 0, 15));
	Soldier.init(&core, "Models/Soldier1.gem", Soldierinslocation, 1);

	AnimationInstance Soldierins;
	Soldierins.animation = &Soldier.animation;
	Soldierins.currentAnimation = "idle";



	while (run)
	{
		//win.clipMouseToWindow();

		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();

		win.processMessages();

		sampler.bind(&core);




		if (win.keys['N']) {
			win.showCursor();
		}
		else if(win.keys['H']) {
			win.hideCursor();
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
		
		// Pass 1
		//core.settoGbuffer();

		//sphere.draw(&core, shaders.find("definedshapeG"), &W, &camera.vp, Vec3(1, 1, 1));

		//core.devicecontext->PSSetShaderResources(0, 3, core.shaderResourceViews);

		// Pass2
		core.settoBackbuffer();

		sphere.draw(&core, shaders.find("definedshapeG"), &W, &camera.vp, Vec3(1, 1, 1));

		//p.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1, 1, 1), &textures);

		//pine.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(0.005f, 0.005f, 0.005f), &textures);

		//TRexins.update("Run", dt);

		//TRex.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(1,1,1), &TRexins, &textures);


		//Soldierins.update("idle", dt);

		//Soldier.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(0.02,0.02,0.02), &Soldierins, &textures);


		core.present();
	}

}
