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
	int width, height, mode, lane;
	int lost = 0;
	int checkPoint = 1;
	int score = 0;
	//Lane
	vector<LaneInfo> SpawnArray;
public:
	int getLane() const {
		return lane-1;
	}
	Level(int lane, int mode); 
	Level() : Level(5,2){}
	LaneInfo& getLane(int i);
	void ResetLane();
	void CheckEntity();
	void spawnRandom();
	void LooseGame();
	const Entity* getPlayer() const;
	vector<Animator*> getAnimation(int type) const;
	char** generateMap(int width, int height) const;
	static char** reset(int width, int height);
	static void deleteMap(char**& old, int width);

	vector<Entity*> getEntities() const;

	int getWidth() const;

	int getHeight() const;
	int getScore() const;

	friend class GameCore;
};
#endif // LEVEL_H_INCLUDED