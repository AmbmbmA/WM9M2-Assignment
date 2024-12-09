#pragma once

#include "Window.h"
#include "Adapter.h"
#include "DXcore.h"
#include "Mesh.h"
#include "Shaders.h"
#include "GamesEngineeringBase.h"
#include "Camera.h"
#include "Collision.h"

enum NPCname { TRex, Soldier };

const int DEMAGE = 3;
const int MAXNPC = 50;
const float SPAWNGAP = 10;


class NPC {
public:

	AnimatedModel npc;
	vector<Vec3> npcinslocation;
	AnimationInstance npcani;
	string filename;
	float health = 1000;
	bool chase = false;
	bool roar = false;
	bool attack = false;
	bool move = false;
	bool isattacked = false;
	bool beenattacked = false;

	Vec3 modelfaward;
	Vec3 dir = Vec3(1, 0, 1);

	float timeElapsed = 0;

	~NPC() {}

	void init(DXcore* core, string _filename, Vec3 position) {
		filename = _filename;
		npcinslocation.push_back(position);
		npc.init(core, filename, npcinslocation, 1);
		npcani.animation = &npc.animation;
	}

	void positionupdate(Camera* camera, Window* win, string* animation, float dt) {

		Vec3 position = npcinslocation[0];
		Vec3 camerap = Vec3(camera->position.x, 0, camera->position.z);
		float distance = (position - camerap).getlength();

		if (isattacked && !beenattacked) beenattacked = true;

		if (distance > 600) beenattacked = false;

		if (distance < 400) {
			roar = true;
			move = false;
		}
		else {
			roar = false;
		}

		if (distance < 350 || beenattacked) {
			chase = true;
			move = false;
			roar = false;
		}
		else {
			chase = false;
		}

		if (distance < 30) {
			attack = true;
			chase = false;
			move = false;
			roar = false;
		}
		else {
			attack = false;
		}

		if (health <= 0) {
			*animation = "death";
			if (!roar && !chase && !attack) {
				modelfaward = dir.normalize();
			}
		}
		else if (roar) {

			*animation = "roar";

			modelfaward = -(position - camerap).normalize();

		}
		else if (chase) {
			*animation = "Run";
			Vec3 newposition = position + (camerap - position).normalize() * 0.4;
			npcinslocation.clear();
			npcinslocation.push_back(newposition);

			modelfaward = -(position - camerap).normalize();
		}
		else if (attack) {

			*animation = "attack";

			camera->health -= 20;
			modelfaward = -(position - camerap).normalize();

		}
		else if (move) {
			timeElapsed += dt;



			if (timeElapsed <= 5 + rand() % 26) {

				*animation = "walk";

				Vec3 newposition = position + dir.normalize() * 0.4;
				npcinslocation.clear();
				npcinslocation.push_back(newposition);

				modelfaward = dir.normalize();
			}
			else {
				*animation = "Idle";
				move = false;
				timeElapsed = 0.0f;

			}
		}
		else {
			timeElapsed += dt;

			if (timeElapsed <= 2 + rand() % 9) {
				*animation = "Idle";
				modelfaward = dir.normalize();

			}
			else {
				*animation = "walk";
				move = true;
				timeElapsed = 0.0f;

				float randomx = (-1 + rand() % 3);
				if (randomx == 0) randomx = 1;
				float randomz = (-1 + rand() % 3);

				Vec3 change = Vec3(randomx, 0, randomz);
				dir = change;
				modelfaward = dir.normalize();
			}
		}


	}

	void update(Window* win, float dt, DXcore* core, Camera* camera) {

		string animation;

		positionupdate(camera, win, &animation, dt);

		if (isattacked) {
			health -= DEMAGE;
		}

		npcani.update(animation, dt);

		for (auto mesh : npc.meshes) {
			mesh.updateinstanceBuffer(core, npcinslocation);
		}

	}



	void draw(DXcore* core, ShaderManager* shaders, Camera* camera, Vec3 Scal, TextureManager* textures) {

		Vec3 left = modelfaward.Crossright(Vec3(0, 1, 0)).normalize();
		Matrix w = Matrix::Transformationto(left, Vec3(0, 1, 0), modelfaward, Vec3(0, 0, 0));

		npc.draw(core, shaders->find("animated"), &w, &camera->vp, Scal, &npcani, textures);

	}

	void save(ofstream& save) {
		save << npcinslocation[0].x << endl;
		save << npcinslocation[0].y << endl;
		save << npcinslocation[0].z << endl;
		save << health << endl;
		save << chase << endl;
		save << roar << endl;
		save << move << endl;
		save << isattacked << endl;
		save << beenattacked << endl;
		save << modelfaward.x << endl;
		save << modelfaward.y << endl;
		save << modelfaward.z << endl;
		save << dir.x << endl;
		save << dir.y << endl;
		save << dir.z << endl;
		save << timeElapsed << endl;
	}

};



class Spawn {
public:
	vector<NPC*> npcmanage;

	float timeElapsed = 0.0f; // time passed since last generate
	float timeThreshold = SPAWNGAP; // generate time gap

	vector<float> deathanimationtime;
	int maxnum = MAXNPC;



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

			int randomX = camera->position.x;
			int randomZ = camera->position.z;

			Vec3 position = Vec3(randomX, 0, randomZ);
			Vec3 camerap = Vec3(camera->position.x, 0, camera->position.z);

			while ((position - camerap).getlength() <= 1000) {
				randomX = camera->position.x - 1500 + rand() % 3001;
				randomZ = camera->position.z - 1500 + rand() % 3001;
				position = Vec3(randomX, 0, randomZ);
			}
			//position = Vec3(1, 0, 1);


			NPC* n = new NPC;

			n->init(core, getfilename(name), position);

			npcmanage.push_back(n);
			deathanimationtime.push_back(0);
			timeElapsed = 0.0f; //reset
		}
	}




	void update(Window* win, float dt, DXcore* core, Camera* camera) {

		generate(core, camera, dt, randomnpc());

		for (int i = 0; i < npcmanage.size(); i++) {

			NPC* npc = npcmanage[i];

			Collision collision;
			collision.setray(camera->position, camera->movedirforward, 400);
			collision.setsphere1(npc->npcinslocation[0], 25);

			npc->isattacked = false;
			if (win->mouseButtons[0]) {


				if (collision.raysphere1check()) {
					npc->isattacked = true;

				}

			}

			collision.setsphere2(camera->position, 5);
			Vec3 contra;
			if (collision.spherecheck(contra)) {
				camera->position.x -= contra.x;
				camera->position.z -= contra.z;
			}

			float distance = (Vec3(camera->position.x, 0, camera->position.z) - npc->npcinslocation[0]).getlength();

			npc->update(win, dt, core, camera);
			if (npc->health <= 0) {
				deathanimationtime[i] += dt;

			}
			if (deathanimationtime[i] >= 2.8 || distance >= 1500) {
				npc->~NPC();
				npcmanage.erase(npcmanage.begin() + i);
				deathanimationtime.erase(deathanimationtime.begin() + i);
			}

		}

	}

	void draw(DXcore* core, ShaderManager* shaders, Camera* camera, Vec3 Scal, TextureManager* textures) {
		for (auto npc : npcmanage) {

			npc->draw(core, shaders, camera, Scal, textures);
		}

	}

	void save(ofstream& save) {
		save << timeElapsed << endl;

		save << deathanimationtime.size() << endl;
		for (auto time : deathanimationtime) {
			save << time << endl;
		}

		save << npcmanage.size() << endl;
		for (auto npc : npcmanage) {
			npc->save(save);
		}

	}

	void load(DXcore* core, ifstream& load) {
		load >> timeElapsed;

		int size;

		load >> size;
		deathanimationtime.clear();
		for (int i = 0; i < size; i++) {
			float time;
			load >> time;
			deathanimationtime.push_back(time);
		}
		load >> size;
		npcmanage.clear();
		for (int i = 0; i < size; i++) {
			npcload(core, load);
		}
	}

	void npcload(DXcore* core, ifstream& load) {
		float x, y, z;
		load >> x;
		load >> y;
		load >> z;
		Vec3 position = Vec3(x, y, z);
		NPC* npc = new NPC;
		npc->init(core, "Models/TRex.gem", position);
		load >> npc->health;
		load >> npc->chase;
		load >> npc->roar;
		load >> npc->move;
		load >> npc->isattacked;
		load >> npc->beenattacked;
		load >> x;
		load >> y;
		load >> z;
		npc->modelfaward = Vec3(x, y, z);
		load >> x;
		load >> y;
		load >> z;
		npc->dir = Vec3(x, y, z);
		load >> npc->timeElapsed;
		npcmanage.push_back(npc);
	}
};

