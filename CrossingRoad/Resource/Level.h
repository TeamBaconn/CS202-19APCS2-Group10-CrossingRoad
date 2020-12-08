#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define LANE_WIDTH 120
#define LANE_HEIGHT 8
#define LANE_DISTANCE 5

#define CAM_LOCK_X true
#define CAM_LOCK_Y false

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 60

#define INGAME_WIDTH 120
#define INGAME_HEIGHT 40

#define ANIMATION "./Resource/Animation/"

#include "Entity/Entity.h"
#include <random>


enum class GameState { MENU, PAUSE, PLAYING };

class Level {
private:
	vector<Animator*> anim_list;
	Entity* player;
	vector<Entity*> entities;
	int width, height, mode, lane;
	//Lane
public:
	void ReplaceAll(string& c, char f, char t) {
		for (int i = 0; i < c.length(); i++) if (c[i] == f) c[i] = t;
	}
	Animator* readAnimator(string path, int id) {
		ifstream info((string)ANIMATION + "/" + path);
		if (!info.is_open()) return nullptr;
		int speed, set;
		string k;
		info >> speed >> set;
		info.ignore();
		vector<Key> key;
		Key kc;
		int max = -1;
		while (!info.eof()) {
			getline(info, k);
			ReplaceAll(k, ' ', '!');//Set background opacity to 0
			if (max == -1 || max < k.length()) max = k.length();
			if (k == "X") {
				key.push_back(kc);
				kc = *new Key();
				continue;
			}
			kc.key.push_back(k);
		}
		return new Animator(key, speed, id, max, set);
	}
	Level() = default;
	Level(int lane, int mode) {
		//Load resource 
		cout << "Begin load data" << endl;
		ifstream info;
		info.open((string)ANIMATION + "/setting.txt");
		string c;
		int id;
		while (!info.eof()) {
			info >> id;
			info.ignore();
			getline(info, c);
			anim_list.push_back(readAnimator(c, id));
		}

		this->lane = lane;
		this->mode = mode;
		this->width = LANE_WIDTH;
		this->height = lane*LANE_HEIGHT;

		//Spawn
		player = new Player
		(Position(LANE_WIDTH/2,5),getAnimation(HUMAN_ID)[0]);

		//Thread rieng de spawn quai vat
		entities.push_back(player);
		for (int i = 0; i < lane; i++) {
			vector<Animator*> anim = getAnimation(CAR_ID);
			entities.push_back(new Car
			(Position(0, i*LANE_HEIGHT+LANE_HEIGHT/2), anim[rand() % anim.size()]));
		}
	}
	const Entity* getPlayer() const{
		return player;
	}
	vector<Animator*> getAnimation(int type) const{
		vector<Animator*> list;
		for (int i = 0; i < anim_list.size(); i++)
			if (anim_list[i]->id == type) list.emplace_back(anim_list[i]);
		return list;
	}
	char** generateMap() const {
		char** map = reset(width, height);
		for (int i = 0; i < width; i++) 
			for (int j = 0; j < height; j++) 
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1) 
					map[i][j] = '#';
		for (int i = 1; i < lane; i++)
			for (int j = 0; j < width; j+=LANE_DISTANCE) map[j][i * LANE_HEIGHT] = '-';
		return map;
	}
	static char** reset(int width, int height) {
		char** map = new char* [width];
		for (int i = 0; i < width; ++i) {
			map[i] = new char[height];
			for (int j = 0; j < height; j++)
				map[i][j] = ' ';
		}
		return map;
	}
	static void deleteMap(char** old, int width) {
		if (old == nullptr) return;
		for (int i = 0; i < width; i++) delete old[i];
		delete old;
	}

	vector<Entity*> getEntities() const {
		return entities;
	}

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}
	friend class GameCore;
};
#endif // LEVEL_H_INCLUDED