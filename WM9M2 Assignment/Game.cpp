#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"


const int WINDOWSIZE[2] = { 1024, 720 };

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

	Matrix view, to, vp;
	Vec3 from;
	float fov = 90.0f;

	Matrix planeWorld = Matrix::Transformationto(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), Vec3(0, 0, 0));

	Matrix projection = Matrix::Perspectiveprojectionz01(1, 100, fov, (float)WINDOWSIZE[0] / (float)WINDOWSIZE[1]);

	Window win;
	win.init("Cindy's Adventure", WINDOWSIZE[0], WINDOWSIZE[1]);
	bool run = true;

	DXcore core;
	core.init(win.width, win.height, win.hwnd, false);

	Triangle triangle;
	triangle.init(&core);

	Triangle triangle2;
	triangle2.init2(&core);

	Plane p;
	p.init(&core);

	Cube c;
	c.init(&core);


	Shader shader;
	shader.init("static", &core);

	//Shader shader;
	//shader.init("static", &core);

	float camerax = 0;
	float cameraz = 0;

	while (run)
	{
		float dt = timer.dt();
		time += dt;

		float u = 200 * dt;
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


		from = Vec3(10 * cosf(time), 5, 10 * sinf(time));

		//from = Vec3(camerax, 10, cameraz);
		view = view.LookAt(from, Vec3(0, 0, 0), Vec3(0, 1, 0));

		vp = projection * view;

		shader.updateConstantVS("staticMeshBuffer", "W", &planeWorld);
		shader.updateConstantVS("staticMeshBuffer", "VP", &vp);

		shader.apply(&core);

		c.mesh.draw(&core);
		//p.mesh.draw(&core);

		//triangle.draw(&core);
		//triangle2.draw(&core);


		core.present();
	}

}


//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
//{
//	Window win;
//	DXcore core;
//	Shader shader_animated, shader_static;
//	Model_static tree;
//	Plane plane;
//	Model_animated trex;
//	AnimationInstance instance;
//	GamesEngineeringBase::Timer timer;
//	float WIDTH = 1024.f;
//	float HEIGHT = 1024.f;
//	Matrix worldMatrix_plane, worldMatrix_tree, worldMatrix_trex;
//	Matrix view, projection, vp;
//	Vec3 from;
//	win.create(1024, 1024, "My Window");
//	core.init(1024, 1024, win.hwnd, false);
//	shader_animated.init("VertexShader_anim.txt", "PixelShader_anim.txt", &core);
//	shader_static.init("VertexShader_1125.txt", "PixelShader_1125.txt", &core);
//	tree.init("acacia_003.gem", &core);
//	plane.init(&core);
//	trex.init("TRex.gem", &core);
//	float time = 0.f;
//	float fov = 45.0f;
//	worldMatrix_tree = Matrix::worldMatrix(Vec3(-4, 0, -4), Vec3(0.03, 0.03, 0.03), 0, 0, 0);
//	worldMatrix_trex = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), 0, 0, 0);
//	worldMatrix_plane = Matrix::worldMatrix(Vec3(0, 0, 0), Vec3(1, 1, 1), 0, 0, 0);
//	projection.perspectiveProjection(1024.f / 1024.f, tan(fov * 5 * M_PI / 180));
//	//initialize animation instance
//	instance.animation = &trex.animation;
//	instance.currentAnimation = "Run";
//	while (true) {
//		win.processMessages();
//		core.clear();
//		float dt = timer.dt();
//		instance.update("Run", dt);
//		from = Vec3(20 * cos(time), 10, 20 * sinf(time));
//		view = view.lookAtMat(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
//		vp = view * projection;
//		// draw plane
//		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_plane);
//		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
//		shader_static.apply(&core);
//		plane.mesh.draw(&core);
//		// draw tree
//		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "W", &worldMatrix_tree);
//		shader_static.updateConstantVS("StaticModel", "staticMeshBuffer", "VP", &vp);
//		shader_static.apply(&core);
//		tree.draw(&core);
//		// draw trex
//		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "bones", instance.matrices);
//		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "W", &worldMatrix_trex);
//		shader_animated.updateConstantVS("Animated", "staticMeshBuffer", "VP", &vp);
//		shader_animated.apply(&core);
//		trex.draw(&core);
//		core.present();
//		time += dt;
//	}
//}