#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <Windows.h>
#include <time.h>

#define GAME_RATE 50

#define LANE_WIDTH 120
#define LANE_HEIGHT 8
#define LANE_DISTANCE 5

#define MIN_SPAWN_TIME 1000
#define SPAN_TIME 8000

#define CAM_LOCK_X true
#define CAM_LOCK_Y false

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 60

#define INGAME_WIDTH 120
#define INGAME_HEIGHT 30

#include "Entity/Entity.h"
#include <random>

struct LaneInfo {
	int time;
	bool toRight;
	LaneInfo(int time) {
		this->time = time;
		toRight = rand() % 2;
	}
};

enum class GameState { MENU, PAUSE, PLAYING };

class Level {
private:
	vector<Animator*> anim_list;
	Entity* player;
	vector<Entity*> entities;
	int width, height, mode, lane;
	bool lost = false;
	int checkPoint = 1;
	int score = 0;
	//Lane
	vector<LaneInfo> SpawnArray;
public:
	int getLane() const {
		return SpawnArray.size();
	}
	Level() = default;
	Level(int lane, int mode) {
		// debug
		mode = 1;
		lane = 2;
		//Load resource
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
		this->height = lane * LANE_HEIGHT;

		//Spawn
		player = new Player
		(Position(LANE_WIDTH / 2, 2), getAnimation(HUMAN_ID)[0]);

		Animator* house = getAnimation(3)[0];
		entities.push_back(new Prop(Position(house->getWidth() / 2, 5), house));

		ResetLane();

		// quai vat initialize
		srand(time(NULL));
		entities.push_back(player);
	}
	void ResetLane() {
		for (int i = 0; i < entities.size(); ++i)
			if (entities[i] != player && entities[i]->isCar()) entities[i]->remove = true;

		width = LANE_WIDTH;
		height = lane * LANE_HEIGHT;

		SpawnArray.clear();
		for (int i = 1; i < lane; i++) {
			LaneInfo info(0);
			SpawnArray.push_back(info);
			Position pos(info.toRight ? 3 : -3+width, i*LANE_HEIGHT+LANE_HEIGHT*4/5);
			entities.push_back(new Light(pos,getAnimation(2)[0],nullptr));
		}
	}
	void CheckEntity() {
		for (int i = 0; i < entities.size(); i++)
			if (entities[i] != player) {
				if (entities[i]->Behavior(GAME_RATE, *this)) entities[i]->remove = true;
			}
			else if (entities[i] == player) {
				// if win delete all Car instances
				if (entities[i]->Behavior(GAME_RATE, *this)) {
					++mode;
					if (mode % 3)
						++lane;

					ResetLane();

					score += 20;
					checkPoint = 1;

					break;
				}
				else if (entities[i]->GetPos().y > checkPoint * LANE_HEIGHT) {
					score += 10;
					++checkPoint;
				}
			}

		//This code to remove entity
		int s = entities.size();
		for (int i = s - 1; i >= 0; --i) {
			if (entities[i]->remove)
				entities.erase(entities.begin() + i);
		}
		spawnRandom();
	}
	void spawnRandom() {
		for (int i = 0; i < SpawnArray.size(); ++i) {
			SpawnArray[i].time -= GAME_RATE;
			if (SpawnArray[i].time < 0) {
				SpawnArray[i].time = MIN_SPAWN_TIME + rand() % (SPAN_TIME / mode);
				vector<Animator*> anim = getAnimation(CAR_ID);

				Animator* e = anim[rand() % anim.size()];

				Position p(0, (i+1) * LANE_HEIGHT + LANE_HEIGHT / 2);
				if (!SpawnArray[i].toRight) p = Position(INGAME_WIDTH + e->getWidth(), (i+1) * LANE_HEIGHT + LANE_HEIGHT / 2);

				entities.push_back(new Car(p, e, !SpawnArray[i].toRight));
			}
		}
	}
	void LooseGame() {
		lost = true;
	}
	const Entity* getPlayer() const {
		return player;
	}
	vector<Animator*> getAnimation(int type) const {
		vector<Animator*> list;
		for (int i = 0; i < anim_list.size(); i++)
			if (anim_list[i]->id == type) list.emplace_back(anim_list[i]);
		return list;
	}
	char** generateMap(int width, int height) const {
		char** map = reset(width, height);
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
					map[i][j] = '#';
		for (int i = 1; i < lane; i++)
			for (int j = 0; j < width; j += LANE_DISTANCE) map[j][i * LANE_HEIGHT] = '-';
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
	static void deleteMap(char**& old, int width) {
		if (old == nullptr) return;
		for (int i = 0; i < width; i++) {
			delete[] old[i];
			old[i] = nullptr;
		}
		delete[] old;
		old = nullptr;
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
	int getScore() const {
		return score;
	}

	bool isLost() const {
		return lost;
	}

	friend class GameCore;
};
#endif // LEVEL_H_INCLUDED