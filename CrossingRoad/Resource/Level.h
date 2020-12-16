#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <Windows.h>
#include <time.h>

#define GAME_RATE 70

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

#define TRAFFIC_TIME 3000
#define TRAFFIC_GAP 2000

#define LOST_DELAY 3000

#include "Entity/Entity.h"
#include <random>

struct LaneInfo {
	int time, current_traffic;
	bool toRight;
	bool open;
	LaneInfo(int time);
};

enum class GameState { PLAYING, MENU, PAUSE };

class Level {
private:
	vector<Animator*> anim_list;
	Entity* player;
	vector<Entity*> entities;
	string soundName = "blank";
	string otherSoundName = "blank";
	int width, height, mode, lane;
	int lost = 0;
	int checkPoint = 1;
	int score = 0;
	//Lane
	vector<LaneInfo> SpawnArray;
public:
	int getLane() const;
	Level(int lane, int mode);
	Level() : Level(5, 2) {}
	LaneInfo& getLane(int i);
	void ResetLane();
	void CheckEntity();
	void spawnRandom();
	void GameLose();
	const Entity* getPlayer() const;
	vector<Animator*> getAnimation(int type) const;

	Animator* getAnimator(string name);
	char** generateMap(int width, int height) const;
	static char** reset(int width, int height);
	static void deleteMap(char**& old, int width);

	void writeE2File(ofstream& fout, Entity* entity);
	void readF2E(ifstream& fin, Entity*& entity);
	void saveLevel(string name);
	void loadLevel(string name);

	vector<Entity*> getEntities() const;

	int getWidth() const;

	int getHeight() const;
	int getScore() const;
	void setSoundName(string set);
	void setOtherSoundName(string set);

	friend class GameCore;
};
#endif // LEVEL_H_INCLUDED