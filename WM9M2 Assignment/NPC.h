#pragma once

#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"

class NPC {
public:

	AnimatedModel npc;
	vector<Vec3> npcinslocation;
	AnimationInstance npcani;
	float health;

	void init(DXcore* core, string filename, Vec3 position) {
		npcinslocation.push_back(position);
		npc.init(core, filename, npcinslocation, 1);
		npcani.animation = &npc.animation;
	}


	string animationcontrol(Window* win) {
		return "Run";
	}

	void positonupdate(Camera* camera) {
		for (Vec3 position : npcinslocation) {
			//position += (camera->position - position).normalize();
			position += Vec3(1,0,1);
		}
	}

	void update(Window* win, float dt, DXcore* core, Camera* camera) {



		npcani.update(animationcontrol(win), dt);

		//npcinslocation.clear();
		//npcinslocation.push_back(positonupdate(camera));

		//positonupdate(camera);

		for (auto mesh : npc.meshes) {
			Vec3 position = npcinslocation[0];
			Vec3 newposition = position + (Vec3(camera->position.x,0, camera->position.z) - position).normalize() * 0.2;
			npcinslocation.clear();
			npcinslocation.push_back(newposition);
			mesh.updateinstanceBuffer(core, npcinslocation);
		}

	}

	void draw(DXcore* core, ShaderManager* shaders, Camera* camera, Vec3 Scal, TextureManager* textures) {
		Matrix w;
		npc.draw(core, shaders->find("animated"), &w, &camera->vp, Scal, &npcani, textures);

	}


};

enum NPCname { TRex, Soldier };

class Spawn {
public:
	vector<NPC*> npcmanage;

	float timeElapsed = 0.0f; // time passed since last generate
	float timeThreshold = 20; // generate time gap

	int maxnum = 50;



	string getfilename(NPCname name) {
		switch (name)
		{
		case TRex:
			return "Models/TRex.gem";
			break;
		case Soldier:
			return "";
			break;
		default:
			return"";
			break;
		}
	}

	NPCname randomnpc() {

		return TRex;
	}

	void generate(DXcore* core, Camera* camera, float dt, NPCname name) {
		timeElapsed += dt;
		bool full = false;
		if (npcmanage.size() >= maxnum) {
			full = true;
		}
		if (timeElapsed >= timeThreshold && (!full)) {
			//random position

			int randomX, randomZ;
			randomX = camera->position.x -100 + rand() % 100;
			randomZ = camera->position.z -100 + rand() % 100;
			Vec3 position = Vec3(randomX, 0, randomZ);

			NPC* n = new NPC;
			n->init(core, getfilename(name), position);

			npcmanage.push_back(n);

			timeElapsed = 0.0f; //reset
		}
	}




	void update(Window* win, float dt, DXcore* core, Camera* camera) {

		generate(core, camera, dt, randomnpc());

		for (auto npc : npcmanage) {

			npc->update(win, dt, core, camera);
		}

	}

	void draw(DXcore* core, ShaderManager* shaders, Camera* camera, Vec3 Scal, TextureManager* textures) {
		for (auto npc : npcmanage) {

			npc->draw(core, shaders, camera, Scal, textures);
		}

	}



};