#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include<vector>
#include<string>

#define HUMAN_ID 0
#define CAR_ID 1
#define MENU_ID 2

using namespace std;

struct Frame {
	vector<string> key;
};
class Animator {
private:
	int id, width, set, speed;
	vector<Frame> animation_set;
public:
	Animator() = default;
	Animator(vector<Frame>& animation_set, int speed, int id, int width, int set) {
		this->width = width;
		this->animation_set = animation_set;
		this->id = id;
		this->set = set;
		this->speed = speed;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return animation_set[0].key.size();
	}
	~Animator() = default;

	friend class Level;
	friend class AnimatorData;
};
class AnimatorData {
private:
	int k, current_time, base, lim, refresh;
	Animator* animator;
public:
	AnimatorData() = default;
	AnimatorData(Animator* animator) {
		this->animator = animator;
		k = 0;
		current_time = 0;
		refresh = 0;
		base = 0;
		lim = animator->animation_set.size() / animator->set;
	}
	void push(int frame) {
		current_time += frame;
		refresh += frame;
		if (current_time >= animator->speed) {
			current_time = 0;
			k++;
		}
	}
	void changeAnimation(int i) {
		base = i * animator->animation_set.size() / animator->set;
		lim = base + animator->animation_set.size() / animator->set;
		if (k < base && lim >= k) k = base;
		refresh = 0;
	}
	Frame getFrame() {
		if (k >= lim) {
			if (refresh > 50) {
				base = 0;
				lim = animator->animation_set.size() / animator->set;
			}
			k = base;
		}
		return animator->animation_set[k];
	}
};

class Position {
public:
	float x, y;
	Position();
	Position(float x, float y);

	~Position();

	Position operator+(const Position& p);
	void operator=(const Position& p);
};

class Entity {
protected:
	Position pos;
	Animator* animator;
	AnimatorData data;
	bool remove = false;
public:
	//Constructor
	Entity();
	Entity(Position pos, Animator* animator);

	~Entity();

	//Functions
	Position Move(Position pos);

	virtual bool Behavior(int rate, Level& level) {
		data.push(rate);
		return false;
	}

	virtual bool isCar() { return false; }

	void changeBase(int id) {
		data.changeAnimation(id);
	}
	Position GetPos() const {
		return pos;
	}
	Animator* getAni() {
		return animator;
	}
	friend class Graphic;
	friend class Level;
};

// 1
class Car : public Entity {
public:
	bool toRight;
	Car();
	Car(Position pos, Animator* animator);
	Car(Position pos, Animator* animator, bool toRight);

	bool Behavior(int rate, Level& level);
	bool isCar();
};

class Player : public Entity {
public:
	Player();
	Player(Position pos, Animator* animator);
	bool Behavior(int rate, Level& level);
};

class Prop : public Entity {
public:
	Prop();
	Prop(Position pos, Animator* animator);
};
#endif // ENTITY_H_INCLUDED