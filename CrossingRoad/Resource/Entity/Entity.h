#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "../Graphic/Animator.h"

#define HUMAN_ID 0
#define CAR_ID 1

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

class Light : public Entity {
private:
	bool* on;
public:
	Light();
	Light(Position pos, Animator* animator, bool* on);
	bool isCar();
};
#endif // ENTITY_H_INCLUDED