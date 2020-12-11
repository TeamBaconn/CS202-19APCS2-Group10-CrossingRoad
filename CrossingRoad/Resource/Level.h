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


enum class GameState { MENU, PAUSE, PLAYING };

class Level {
private:
	vector<Animator*> anim_list;
	Entity* player;
	vector<Entity*> entities;
	int width, height, mode, lane;
	bool lost = false;
	vector<int> score;
	//Lane
	vector<int> SpawnArray;
public:
	int& getMode() { return mode; }
	Level() = default;
	Level(int lane, int mode) : score(lane + 1, 0) {
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

		for (int i = 1; i < lane; i++) SpawnArray.push_back(0);

		// quai vat initialize
		srand(time(NULL));
		entities.push_back(player);
	}
	void CheckEntity() {
		// check if player has won, to next level (mode)
		if (entities.size() == 1) {
			spawnRandom();
			return;
		}

		for (int i = 0; i < entities.size(); i++)
			if (entities[i] != player) {
				if (entities[i]->Behavior(GAME_RATE, *this)) entities[i]->remove = true;
			}
			else if (entities[i] == player) {

				unsigned int pos = find(score.begin(), score.end(), 0) - score.begin();
				pos = pos ? pos : 1;
				if (entities[i]->GetPos().y > pos * LANE_HEIGHT) {
					score[pos] = 10;
				}

				// if win delete all Car instances
				if (entities[i]->Behavior(GAME_RATE, *this)) {
					for (int i = 0; i < score.size() - 1; ++i) {
						score[(score.size() - 1)] += score[i];
						score[i] = 0;
					}
					score[(score.size() - 1)] += 20;
					for (int i = 0; i < entities.size(); ++i)
						if (entities[i] != player && entities[i]->isCar()) entities[i]->remove = true;
					break;
				}
			}

		int s = entities.size();
		for (int i = s - 1; i >= 0; --i) {
			if (entities[i]->remove)
				entities.erase(entities.begin() + i);
		}
		spawnRandom();
	}
	void spawnRandom() {
		for (int i = 1; i < lane; ++i) {
			SpawnArray[i - 1] -= GAME_RATE;
			if (SpawnArray[i - 1] < 0) {
				SpawnArray[i - 1] = MIN_SPAWN_TIME + rand() % (SPAN_TIME / mode);
				vector<Animator*> anim = getAnimation(CAR_ID);
				entities.push_back(new Car
				(Position(0, i * LANE_HEIGHT + LANE_HEIGHT / 2), anim[rand() % anim.size()]));
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
	char** generateMap() const {
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
	static void deleteMap(char** old, int width) {
		if (old == nullptr) return;
		for (int i = 0; i < width; i++) delete[] old[i];
		delete[] old;
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
		unsigned int sum = 0;
		for (int i = 0; i < score.size(); ++i) sum += score[i];
		return sum;
	}

	bool isLost() const {
		return lost;
	}

	friend class GameCore;
};
#endif // LEVEL_H_INCLUDED