﻿#include "Window.h"
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

	Matrix W = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));

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
	textures.load(&core, "grassland.jpg");
	textures.load(&core, "grassland1.jpg");

	textures.load(&core, "sky.png");

	textures.load(&core, "stump01.png");
	textures.load(&core, "bark09.png");
	textures.load(&core, "pine branch.png");
	textures.load(&core, "stump01_normal.png");
	textures.load(&core, "bark09_normal.png");
	textures.load(&core, "pine branch_normal.png");

	textures.load(&core, "T-rex_Base_Color.png");
	textures.load(&core, "T-rex_Normal_OpenGL.png");



	textures.load(&core, "arms_1_Albedo.png");
	textures.load(&core, "AC5_Albedo.png");
	textures.load(&core, "AC5_Bullet_Shell_Albedo.png");
	textures.load(&core, "AC5_Collimator_Albedo.png");
	textures.load(&core, "AC5_Collimator_Glass_Albedo.png");





	//textures.load(&core, "MaleDuty_3_OBJ_Serious_Packed0_Diffuse.png");



	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Gloss.png");
	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Normal.png");
	//Tte.load(&core, "MaleDuty_3_OBJ_Happy_Packed0_Specular.png");


	Plane ground;
	vector<Vec3> groundinslocation;
	groundinslocation.push_back(Vec3(0, 0, 0));
	ground.init(&core, 5000, groundinslocation, 1);

	Sphere Sky;
	vector<Vec3> Skyinslocation;
	Skyinslocation.push_back(Vec3(0, 0, 0));
	Sky.init(&core, 1000, 1000, FARPLANE - 20, Skyinslocation, 1);

	// pine

	StaticModel pine;
	vector<Vec3> pineinslocation;
	int pinerangemin = -500;
	int pinerangemax = 500;
	int pinenum = 200;
	//for (int i = 0; i < pinenum; i++) {
	//	int x = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
	//	int y = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
	//	pineinslocation.push_back(Vec3(x, 0, y));
	//}
	//pine.init(&core, "Models/pine.gem", pineinslocation, pinenum);


	pineinslocation.push_back(Vec3(3, 0, 3));

	pine.init(&core, "Models/pine.gem", pineinslocation, 1);


	// cube

	StaticModelwithtiling cube;
	vector<Vec3> cubeinslocation;
	cubeinslocation.push_back(Vec3(-5, 7, -5));
	cube.init(&core, "Models/cube.gem", 3, cubeinslocation, 1);


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


	AnimatedModel Soldier;
	vector<Vec3> Soldierinslocation;
	Soldierinslocation.push_back(Vec3(0.0f, 0.0f, 0.0f));
	Soldier.init(&core, "Models/Soldier1.gem", Soldierinslocation, 1);

	AnimationInstance Soldierins;
	Soldierins.animation = &Soldier.animation;
	Soldierins.currentAnimation = "idle";

	AnimatedModel ShootingArm;
	vector<Vec3> ShootingArminslocation;
	ShootingArminslocation.push_back(Vec3(0.0f, 8, 0.0f));
	ShootingArm.init(&core, "Models/Automatic_Carbine_5.gem", ShootingArminslocation, 1);

	AnimationInstance ShootingArmins;
	ShootingArmins.animation = &ShootingArm.animation;
	ShootingArmins.currentAnimation = "Armature|07 Run";

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
		if (win.keys['U']) {
			ClipCursor(NULL);
		}
		else {
			win.clipMouseToWindow();	
		}

		float dt = timer.dt();
		time += dt;

		float u = 1 * dt;
		core.clear();

		win.processMessages();

		sampler.bind(&core);


		if (win.keys['N']) {
			win.showCursor();
		}
		else if (win.keys['H']) {
			win.hideCursor();
		}



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


		//pine.draw(&core, shaders.find("staticNM"), &W, &camera.vp, Vec3(0.05f, 0.05f, 0.05f), &textures);

		TRexins.update("attack", dt);

		TRex.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(4, 4, 4), &TRexins, &textures);


		

		if (win.mouseButtons[0]) {
			ShootingArmins.update("Armature|08 Fire", dt);	
		}
		else {
			ShootingArmins.update("Armature|07 Run", dt);
		}

		ShootingArminslocation.clear();
		ShootingArminslocation.push_back(Vec3(camera.position.x, camera.position.y, camera.position.z));
		for (auto mesh : ShootingArm.meshes) {
			mesh.updateinstanceBuffer(&core, ShootingArminslocation);
		}


		Matrix cameraW = Matrix::Transformationto(-camera.movedirright, Vec3(0, 1, 0), -camera.movedirforward, Vec3(0, 0, 0));
		cameraW = Matrix::RotationAroundAxis(camera.movedirright, M_PI - camera.theta* M_PI/180) * cameraW;

		ShootingArm.draw(&core, shaders.find("animated"), &cameraW, &camera.vp, Vec3(0.2, 0.2,0.2), &ShootingArmins, &textures);

		//Soldierins.update("idle", dt);

		//Soldier.draw(&core, shaders.find("animated"), &W, &camera.vp, Vec3(0.05, 0.05, 0.05), &Soldierins, &textures);



		ground.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1000, 0.0f, 1000), &textures);

		Skyinslocation.clear();
		Skyinslocation.push_back(Vec3(camera.position.x, 0, camera.position.z));
		Sky.mesh.updateinstanceBuffer(&core, Skyinslocation);
		Sky.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1, -1, 1), &textures);

		core.present();
	}

}
