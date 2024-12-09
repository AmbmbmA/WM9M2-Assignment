#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "NPC.h"
#include "Collision.h"
#include <fstream>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//int screenWidth = GetSystemMetrics(SM_CXSCREEN);
//int screenHeight = GetSystemMetrics(SM_CYSCREEN);

const int WINDOWSIZE[2] = { 1920,1080 }; // 1920,1080  2560,1440
const float FOV = 90;
const float NEARPLANE = 0.1;
const float FARPLANE = 1500;

bool fileexist(const string& filename) {
	ifstream file(filename);
	return file.good();
}

void savegame(string _filename, Camera* camera, vector<Vec3>* pineinslocation, vector<Vec3>* wallinslocationx, vector<Vec3>* wallinslocationz, Spawn* npcspawn) {
	string filename = "Save/" + _filename + ".txt";
	ofstream save;

	save.open(filename, ios::out);

	camera->save(save);

	save << pineinslocation->size() << endl;

	for (auto position : *pineinslocation) {
		save << position.x << endl;
		save << position.y << endl;
		save << position.z << endl;
	}

	save << wallinslocationx->size() << endl;

	for (auto position : *wallinslocationx) {
		save << position.x << endl;
		save << position.y << endl;
		save << position.z << endl;
	}

	save << wallinslocationz->size() << endl;

	for (auto position : *wallinslocationz) {
		save << position.x << endl;
		save << position.y << endl;
		save << position.z << endl;
	}

	npcspawn->save(save);

	save.close();
}

void loadgame(string _filename, DXcore* core, Camera* camera, vector<Vec3>* pineinslocation, vector<Vec3>* wallinslocationx, vector<Vec3>* wallinslocationz, Spawn* npcspawn) {

	string filename = "Save/" + _filename + ".txt";
	if (fileexist(filename)) {
		ifstream load;

		load.open(filename, ios::in);

		camera->load(load);

		int size;
		load >> size;
		pineinslocation->clear();
		for (int i = 0; i < size; i++) {
			Vec3 position;
			load >> position.x;
			load >> position.y;
			load >> position.z;
			pineinslocation->push_back(position);
		}

		load >> size;
		wallinslocationx->clear();
		for (int i = 0; i < size; i++) {
			Vec3 position;
			load >> position.x;
			load >> position.y;
			load >> position.z;
			wallinslocationx->push_back(position);
		}

		load >> size;
		wallinslocationz->clear();
		for (int i = 0; i < size; i++) {
			Vec3 position;
			load >> position.x;
			load >> position.y;
			load >> position.z;
			wallinslocationz->push_back(position);
		}

		npcspawn->load(core, load);

		load.close();
	}



}

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
	//textures.load(&core, "T-rex_Normal_OpenGL.png");


	//Shooting arm
	textures.load(&core, "arms_1_Albedo.png");
	textures.load(&core, "AC5_Albedo.png");
	textures.load(&core, "AC5_Bullet_Shell_Albedo.png");
	textures.load(&core, "AC5_Collimator_Albedo.png");
	textures.load(&core, "AC5_Collimator_Glass_Albedo.png");

	//wall
	textures.load(&core, "rounded-brick1-albedo.png");
	textures.load(&core, "rounded-brick1-normal.png");


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
	Vec3 pineScal = Vec3(0.07f, 0.07f, 0.07f);
	int pinerangemin = -2000;
	int pinerangemax = 2000;
	int pinenum = 3000;
	for (int i = 0; i < pinenum; i++) {
		int x = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
		int y = pinerangemin + rand() % (pinerangemax - pinerangemin + 1);
		pineinslocation.push_back(Vec3(x, 0, y));
	}
	AABB pinebox;
	pine.init(&core, "Models/pine.gem", pineinslocation, pinenum, &pinebox);

	pinebox.max = Matrix::Scaling(pineScal).mulPoint(pinebox.max);
	pinebox.min = Matrix::Scaling(pineScal).mulPoint(pinebox.min);


	// wall

	StaticModelwithtiling wallx;
	StaticModelwithtiling wallz;
	vector<Vec3> wallinslocationx;
	vector<Vec3> wallinslocationz;
	Vec3 wallxScal = Vec3(20, 30, 0.5);
	Vec3 wallzScal = Vec3(0.5, 20, 30);
	int wallrangemin = -2000;
	int wallrangemax = 2000;
	int wallnum = 50;
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
	AABB wallxbox;
	AABB wallzbox;
	wallx.init(&core, "Models/cube.gem", 20, wallinslocationx, wallnum, &wallxbox);
	wallz.init(&core, "Models/cube.gem", 20, wallinslocationz, wallnum, &wallzbox);

	wallxbox.max = Matrix::Scaling(wallxScal).mulPoint(wallxbox.max);
	wallxbox.min = Matrix::Scaling(wallxScal).mulPoint(wallxbox.min);
	wallzbox.max = Matrix::Scaling(wallzScal).mulPoint(wallzbox.max);
	wallzbox.min = Matrix::Scaling(wallzScal).mulPoint(wallzbox.min);


	// Spawn
	Spawn npcspawn;


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


		float u = 1 * dt;
		core.clear();

		win.processMessages();

		sampler.bind(&core);

		// clip, hide mouse
		{

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

		}

		// save and load
		if (win.keys['B']) {
			savegame("save", &camera, &pineinslocation, &wallinslocationx, &wallinslocationz, &npcspawn);
		}
		else if (win.keys['N']) {
			loadgame("save", &core, &camera, &pineinslocation, &wallinslocationx, &wallinslocationz, &npcspawn);
		}


		//quit game
		if (win.keys[VK_ESCAPE]) {
			PostMessage(win.hwnd, WM_CLOSE, 0, 0); //post a message to requst closing the window without blocking
			win.keys[VK_ESCAPE] = false;
		}

		// speed up and squat
		if (win.keys[VK_SHIFT]) {
			camera.speed = 50;
		}
		if (win.keys[VK_CONTROL]) {
			camera.speed = 10;
			camera.position.y = 3;
		}
		else if (!camera.isjump) {
			camera.position.y = 8;
		}

		camera.update(dt);

		// collision 
		{

			Collision collision;
			collision.setsphere1(camera.position, 3);
			for (auto position : pineinslocation) {
				if ((camera.position - position).getlength() < 50) {
					AABB instancebox;
					instancebox.max = pinebox.max + position - Vec3(15, 0, 15); // add so it only culculate the roll
					instancebox.min = pinebox.min + position + Vec3(15, 0, 15);
					collision.setbox1(instancebox);

					Vec3 contra;
					if (collision.box1sphere1check(contra)) {
						camera.position.x += contra.x;
						camera.position.z += contra.z;

					}
				}
			}
			for (auto position : wallinslocationx) {
				if ((camera.position - position).getlength() < 50) {
					AABB instancebox;
					instancebox.max = wallxbox.max + position;
					instancebox.min = wallxbox.min + position;
					collision.setbox1(instancebox);

					Vec3 contra;
					if (collision.box1sphere1check(contra)) {
						camera.position.x += contra.x;
						camera.position.z += contra.z;

					}
				}
			}
			for (auto position : wallinslocationz) {
				if ((camera.position - position).getlength() < 50) {
					AABB instancebox;
					instancebox.max = wallzbox.max + position;
					instancebox.min = wallzbox.min + position;
					collision.setbox1(instancebox);

					Vec3 contra;
					if (collision.box1sphere1check(contra)) {
						camera.position.x += contra.x;
						camera.position.z += contra.z;

					}
				}
			}

		}


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

		// shooting arm
		{

			camera.p = Matrix::Perspectiveprojectionz01(NEARPLANE, FARPLANE, FOV, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
			if (win.mouseButtons[1]) { // aim
				// zoom to aim
				camera.p = Matrix::Perspectiveprojectionz01(NEARPLANE, FARPLANE, FOV / 3, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);
				camera.mousesensity = 50;
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
			else if (win.mouseButtons[0]) { // shoot
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

		}


		npcspawn.update(&win, dt, &core, &camera);

		npcspawn.draw(&core, &shaders, &camera, Vec3(6, 6, 6), &textures);

		// sceen
		{

			//pine

			pine.draw(&core, shaders.find("staticNM"), &W, &camera.vp, pineScal, &textures);

			// wall

			wallx.draw(&core, shaders.find("staticNM"), &W, &camera.vp, wallxScal, &textures, "Textures/rounded-brick1-albedo.png", "Textures/rounded-brick1-normal.png");
			wallz.draw(&core, shaders.find("staticNM"), &W, &camera.vp, wallzScal, &textures, "Textures/rounded-brick1-albedo.png", "Textures/rounded-brick1-normal.png");


			// ground
			ground.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1000, 0.0f, 1000), &textures, "Textures/grassland.jpg");

			// sky
			Skyinslocation.clear();
			Skyinslocation.push_back(Vec3(camera.position.x, 0, camera.position.z));
			Sky.mesh.updateinstanceBuffer(&core, Skyinslocation);
			Sky.draw(&core, shaders.find("static"), &W, &camera.vp, Vec3(1, -1, 1), &textures, "Textures/sky.png");

		}

		core.present();
	}

}
