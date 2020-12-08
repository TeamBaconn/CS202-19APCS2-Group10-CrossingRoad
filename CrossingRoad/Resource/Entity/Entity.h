#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include<vector>
#include<string>

#define HUMAN_ID 0
#define CAR_ID 1
#define DINO 2

using namespace std;

struct Key {
	vector<string> key;
};

class Animator {
private:
	int id,k, time, current_time, width;
	int set;
	int base, lim;
	int refresh;
	vector<Key> frame;
public:
	Animator() = default;
	Animator(vector<Key>& frame, int time, int id, int width, int set) {
		k = 0;
		this->width = width;
		this->frame = frame;
		this->time = time;
		this->id = id;
		this->set = set;
		this->current_time = 0;
		this->refresh = 0;
		base = 0;
		lim = frame.size() / set;
	}
	Key getKey() {
		if (k >= lim) {
			if (refresh > 50) {
				base = 0;
				lim = frame.size() / set;
			}
			k = base;
		}
		return frame[k];
	}
	void push(int frame) {
		current_time += frame;
		refresh += frame;
		if (current_time >= time) {
			current_time = 0;
			k++;
		}
	}
	void changeAnimation(int i) {
		base = i * frame.size() / set;
		lim = base + frame.size() / set;
		if (k < base && lim >= k) k = base;
		refresh = 0;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return getKey().key.size();
	}
	~Animator() = default;
	friend class Level;
};

class Position {
public:
	float x, y;
	Position();
	Position(float x, float y);

	~Position();

	Position operator+(const Position& p);
};

class Entity {
protected:
	Position pos;
	Animator* animator;
public:
	//Constructor
	Entity();
	Entity(Position pos, Animator* animator);

	~Entity();
	
	//Functions
	Position Move(Position pos);
	
	virtual void Behavior(int rate, Level&level) {
		animator->push(rate);
	}

	void changeBase(int id) {
		animator->changeAnimation(id);
	}
	Position GetPos() {
		return pos;
	}
	Animator* getAni() {
		return animator;
	}
	friend class Graphic;
	
};

// 1
class Car : public Entity {
public:
	Car(); 
	Car(Position pos, Animator* animator);
	void Behavior(int rate,  Level& level);
};

class Player : public Entity {
public:
	Player();
	Player(Position pos, Animator* animator);
	void Behavior(int rate, Level&level);
};
#endif // ENTITY_H_INCLUDED