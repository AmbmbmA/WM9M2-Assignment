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

	void update(string mode, float dt, DXcore* core, Vec3 position) {
		npcani.update(mode, dt);

		npcinslocation.clear();
		npcinslocation.push_back(position);
		for (auto mesh : npc.meshes) {
			mesh.updateinstanceBuffer(core, npcinslocation);
		}

	}

	void draw(DXcore* core, ShaderManager& shaders, Camera& camera, Vec3 Scal, TextureManager* textures) {
		Matrix w;
		npc.draw(core, shaders.find("animated"), &w, &camera.vp, Scal, &npcani, textures);

	}


};


class Spawn {
	vector<NPC*> npcmanage;

	int maxnum = 50;

	void animationcontrol() {

	}

	void update(string mode, float dt, DXcore* core, Camera& camera) {

		for (auto npc : npcmanage) {


			Vec3 position = Vec3(0, 0, 0);
			npc->update(mode, dt, core, position);
		}

	}

	void draw(DXcore* core, ShaderManager& shaders, Camera& camera, Vec3 Scal, TextureManager* textures) {
		for (auto npc : npcmanage) {

			npc->draw(core, shaders, camera, Scal, textures);
		}

	}



};