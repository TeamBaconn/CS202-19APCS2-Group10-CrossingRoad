#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#define maxwidth 120
#define maxheight 40

using namespace std;

class Position {
public:
	int x, y;
	Position();
	Position(int x, int y);

	~Position();

	Position operator+(const Position& p);
};

class Entity {
protected:
	Position pos;
	vector<string> animator;
	int width = -1;
public:
	//Constructor
	Entity();
	Entity(Position pos, string type);

	~Entity();
	
	//Functions
	Position Move(Position pos);
	
	virtual void Behavior() {}

	friend class Graphic;
};

class Car : public Entity {
public:
	Car(); 
	Car(Position pos, string type);
	void Behavior();
};

class Player : public Entity {
public:
	Player();
	Player(Position pos, string type);
	void Behavior();
};
#endif // ENTITY_H_INCLUDED