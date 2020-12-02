#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

class Position {
private:
	int x, y;
public:
	Position();
	Position(int x, int y);

	~Position();

	Position operator+(const Position& p);
};

class Entity {
private:
	Position pos;
public:
	//Constructor
	Entity();
	Entity(Position pos);

	~Entity();
	
	//Functions
	Position Move(Position pos);
	
	virtual void Tell();
};

#endif // ENTITY_H_INCLUDED