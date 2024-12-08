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
const float NEARPLANE = 0.1;
const float FARPLANE = 1000;


// Windows program entrance
// prameters are set by the windows system
int WinMain(
	HINSTANCE hInstance, // handle specify the program
	HINSTANCE hPrevInstance, // handle used for specify if there is other program running , only for x16 Windows, now is NULL
	PSTR lpCmdLine, //  for command lines to pass extra info
	int nCmdShow // for initial window setting
) {

	srand(time(0));

	GamesEngineeringBase::Timer timer;
	float time = 0.0f;

	bool run = true;

	Window win;
	win.init("Cindy's Adventure", WINDOWSIZE[0], WINDOWSIZE[1]);

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	Sampler sampler;
	sampler.init(&core);

	Matrix W;

	// Camera
	Matrix projection = Matrix::Perspectiveprojectionz01(NEARPLANE, FARPLANE, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
	Camera camera;
	camera.init(Vec3(0, 8, 0), 90, 0, projection, &win);

	// Shaders
	ShaderManager shaders;
	shaders.load(&core, "definedshape");
	shaders.load(&core, "static");
	shaders.load(&core, "staticNM");
	shaders.load(&core, "animated");

	//shaders.load(&core, "staticNMshadow");
	//shaders.load(&core, "shadow");
	//shaders.load(&core, "definedshapeG");
	//shaders.load(&core, "definedshapeL");


	// Textures
	TextureManager textures;

	// ground
	textures.load(&core, "grassland.jpg");

	// sky
	textures.load(&core, "sky.png");

	// pine
	textures.load(&core, "stump01.png");
	textures.load(&core, "bark09.png");
	textures.load(&core, "pine branch.png");
	textures.load(&core, "stump01_normal.png");
	textures.load(&core, "bark09_normal.png");
	textures.load(&core, "pine branch_Normal.png");

	//TRex
	textures.load(&core, "T-rex_Base_Color.png");
	textures.load(&core, "T-rex_Normal_OpenGL.png");


	//Shooting arm
	textures.load(&core, "arms_1_Albedo.png");
	textures.load(&core, "AC5_Albedo.png");
	textures.load(&core, "AC5_Bullet_Shell_Albedo.png");
	textures.load(&core, "AC5_Collimator_Albedo.png");
	textures.load(&core, "AC5_Collimator_Glass_Albedo.png");

	//wall
	textures.load(&core, "rounded-brick1-albedo.png");
	textures.load(&core, "rounded-brick1-normal.png");

	textures.load(&core, "MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");

	// ground
	Plane ground;
	vector<Vec3> groundinslocation;
	groundinslocation.push_back(Vec3(0, 0, 0));
	ground.init(&core, 5000, groundinslocation, 1);

	// sky
	Sphere Sky;
	vector<Vec3> Skyinslocation;
	Skyinslocation.push_back(Vec3(0, 0, 0));
	Sky.init(&core, 1, 1000, 1000, FARPLANE - 20, Skyinslocation, 1);


	// shooting arm
	AnimatedModel ShootingArm;
	vector<Vec3> ShootingArminslocation;
	ShootingArminslocation.push_back(Vec3(0.0f, 8, 0.0f));
	ShootingArm.init(&core, "Models/Automatic_Carbine_5.gem", ShootingArminslocation, 1);

	AnimationInstance ShootingArmins;
	ShootingArmins.animation = &ShootingArm.animation;
	ShootingArmins.currentAnimation = "Armature|04 Idle";


	// pine

	StaticModel pine;
	vector<Vec3> pineinslocation;
	int pinerangemin = -1000;
	int pinerangemax = 1000;
	int pinenum = 1000;
	for (int i = 0; i < pinenum; i++) {
		int x = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
		int y = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
		pineinslocation.push_back(Vec3(x, 0, y));
	}
	pine.init(&core, "Models/pine.gem", pineinslocation, pinenum);


	//pineinslocation.push_back(Vec3(3, 0, 3));
	//pine.init(&core, "Models/pine.gem", pineinslocation, 1);


	// cube

	StaticModelwithtiling wallx;
	StaticModelwithtiling wallz;
	vector<Vec3> wallinslocationx;
	vector<Vec3> wallinslocationz;
	int wallrangemin = -1000;
	int wallrangemax = 1000;
	int wallnum = 20;
	for (int i = 0; i < wallnum; i++) {
		int x = wallrangemin + rand() % (wallrangemax - wallrangemin + 1);
		int y = wallrangemin + rand() % (wallrangemax - wallrangemin + 1);
		wallinslocationx.push_back(Vec3(x, 0, y));
	}
	for (int i = 0; i < wallnum; i++) {
		int x = wallrangemin + rand() % (wallrangemax - wallrangemin + 1);
		int y = wallrangemin + rand() % (wallrangemax - wallrangemin + 1);
		wallinslocationz.push_back(Vec3(x, 0, y));
	}
	wallx.init(&core, "Models/cube.gem", 20, wallinslocationx, wallnum);
	wallz.init(&core, "Models/cube.gem", 25, wallinslocationz, wallnum);




	// TRex

	AnimatedModel TRex;
	vector<Vec3> TRexinslocation;
	TRexinslocation.push_back(Vec3(10, 0, 10));
	TRexinslocation.push_back(Vec3(5, 0, 5));
	TRexinslocation.push_back(Vec3(20, 0, 20));
	TRex.init(&core, "Models/TRex.gem", TRexinslocation, 3);

	AnimationInstance TRexins;
	TRexins.animation = &TRex.animation;
	TRexins.currentAnimation = "Run";

	// Soldier
	AnimatedModel Soldier;
	vector<Vec3> Soldierinslocation;
	Soldierinslocation.push_back(Vec3(0.0f, 0.0f, 0.0f));
	Soldier.init(&core, "Models/Soldier1.gem", Soldierinslocation, 1);

	AnimationInstance Soldierins;
	Soldierins.animation = &Soldier.animation;
	Soldierins.currentAnimation = "idle";



	// failed shadow mapping
	/*
	Vec3 lightDirection = Vec3(-1.0f, -1.0f, -1.0f).normalize();
	Vec3 lightPosition = Vec3(100.0f, 100.0f, 100.0f);
	Vec3 target = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

	Matrix LightViewMatrix = Matrix::LookAt(lightPosition, target, up);

	Matrix LightProjectionMatrix = Matrix::OrthographicProjection(NEARPLANE,FARPLANE, WINDOWSIZE[0], WINDOWSIZE[1]);
	Matrix LightViewProjMatrix = LightProjectionMatrix * LightViewMatrix;
	*/

	while (run)
	{


		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();

		win.processMessages();

		sampler.bind(&core);

		// clip, hide mouse
		if (win.keys['Y']) {
			win.clipMouseToWindow();
		}
		else if (win.keys['U']) {
			ClipCursor(NULL);
		}
		if (win.keys['H']) {
			win.hideCursor();
		}
		else if (win.keys['J']) {
			win.showCursor();
		}



		//quit game
		if (win.keys[VK_ESCAPE]) {
			PostMessage(win.hwnd, WM_CLOSE, 0, 0); //post a message to requst closing the window without blocking
			win.keys[VK_ESCAPE] = false;
		}



		camera.update(dt);


		// failed deffered shading
		/*

		// Pass 1
		core.settoGbuffer();

		core.devicecontext->PSSetShaderResources(0, 3, core.shaderResourceViews);

		// Pass2
		core.settoBackbuffer();

		*/


		// failed shadow mapping
		/*
		core.settoshadowmap();

		shaders.find("shadow")->updateConstantVS("ShadowMapBuffer", "LightViewProj", &LightViewProjMatrix);

		shaders.find("shadow")->apply(&core);

		pine.draw(&core, shaders.find("shadow"), &W, &camera.vp, Vec3(0.05f, 0.05f, 0.05f), &textures);

		core.settoBackbuffer();
		core.devicecontext->PSSetShaderResources(2, 1, &core.shadowSRV);

		shaders.find("staticNMshadow")->updateConstantVS("staticMeshBuffer", "LightViewProj", &LightViewProjMatrix);
		shaders.find("staticNMshadow")->apply(&core);
		pine.draw(&core, shaders.find("staticNMshadow"), &W, &camera.vp, Vec3(0.05f, 0.05f, 0.05f), &textures);

		*/


		Matrix wallr = Matrix::RotationX(M_PI / 2);
		wallx.draw(&core, shaders.find("staticNM"), &wallr, &camera.vp, Vec3(0.5,30,20), &textures,"Textures/rounded-brick1-albedo.png", "Textures/rounded-brick1-normal.png");
		wallz.draw(&core, shaders.find("staticNM"), &wallr, &camera.vp, Vec3(30,0.5,20), &textures,"Textures/rounded-brick1-albedo.png", "Textures/rounded-brick1-normal.png");


		pine.draw(&core, shaders.find("staticNM"), &W, &camera.vp, Vec3(0.07f, 0.07f, 0.07f), &textures);

		TRexins.update("attack", dt);

		TRex.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(4, 4, 4), &TRexins, &textures);
		//Soldierins.update("idle", dt);

		//Soldier.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(0.05, 0.05, 0.05), &Soldierins, &textures);

		// shooting arm
		if (win.mouseButtons[1]) {
			if (win.mouseButtons[0]) {
				ShootingArmins.update("Armature|13 Zoom Fire", dt);
			}
			else if (win.keys['W'] || win.keys['A'] || win.keys['S'] || win.keys['D'] || win.keys[VK_SPACE]) {
				ShootingArmins.update("Armature|12 Zoom Walk", dt);
			}
			else {
				ShootingArmins.update("Armature|11 Zoom Idle", dt);
			}
		}
		else if (win.mouseButtons[0]) {
			ShootingArmins.update("Armature|08 Fire", dt);
		}
		else if (win.keys['W'] || win.keys['A'] || win.keys['S'] || win.keys['D'] || win.keys[VK_SPACE]) {

			ShootingArmins.update("Armature|07 Run", dt);
		}
		else {
			ShootingArmins.update("Armature|04 Idle", dt);
		}


		ShootingArminslocation.clear();
		ShootingArminslocation.push_back(Vec3(camera.position.x, camera.position.y, camera.position.z));
		for (auto mesh : ShootingArm.meshes) {
			mesh.updateinstanceBuffer(&core, ShootingArminslocation);
		}

		Matrix cameraW = Matrix::Transformationto(-camera.movedirright, Vec3(0, 1, 0), -camera.movedirforward, Vec3(0, 0, 0));
		cameraW = Matrix::RotationAroundAxis(camera.movedirright, M_PI - camera.theta * M_PI / 180) * cameraW;


		ShootingArm.draw(&core, shaders.find("animated"), &cameraW, &camera.vp, Vec3(0.2, 0.2, 0.2), &ShootingArmins, &textures);


		// ground
		ground.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1000, 0.0f, 1000), &textures, "Textures/grassland.jpg");

		// sky
		Skyinslocation.clear();
		Skyinslocation.push_back(Vec3(camera.position.x, 0, camera.position.z));
		Sky.mesh.updateinstanceBuffer(&core, Skyinslocation);
		Sky.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1, -1, 1), &textures, "Textures/sky.png");

		core.present();
	}

}
