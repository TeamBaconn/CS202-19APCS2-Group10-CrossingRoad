#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "../Graphic/Animator.h"

#define HUMAN_ID 0
#define CAR_ID 1
#define LIGHT_ID 2
#define PROP_ID 3


class Position {
public:
	float x, y;
	Position();
	Position(float x, float y);

	~Position();

	Position operator+(const Position& p);
	void operator=(const Position& p);
	int Distance(const Position& p) {
		return sqrt((p.x - x)* (p.x - x)/(3*3) + (p.y - y)* (p.y - y));
	}
};

class Entity {
protected:
	Position pos;
	AnimatorData data;
	bool render = true;
	bool remove = false;
public:
	//Constructor
	Entity();
	Entity(Position pos, Animator* animator);

	~Entity();

	//Functions
	virtual Position Move(Position pos);

	virtual bool Behavior(int rate, Level& level) {
		data.push(rate);
		return false;
	}

	bool canRender() {
		return render;
	}

	bool isRemoved() { return remove; }

	virtual bool isHostile();

	void changeBase(int id);
	Position GetPosition() const;
	AnimatorData& getAnimatorData();
	friend class Level;
};

class Hostile : public Entity {
private:
	int lane;
	int sound_off = 0;
public:
	Hostile();
	Hostile(Position pos, Animator* animator, int lane);

	bool Behavior(int rate, Level& level);
	bool isHostile();
};

class Player : public Entity {
public:
	Player();
	Player(Position pos, Animator* animator);
	bool Behavior(int rate, Level& level);

	Position Move(Position pos);
};

class Prop : public Entity {
public:
	Prop();
	Prop(Position pos, Animator* animator);
};

class Light : public Entity {
private:
	int lane;
public:
	Light();
	Light(Position pos, Animator* animator, int lane);

	bool isHostile();

	bool Behavior(int rate, Level& level);
};
#endif // ENTITY_H_INCLUDED