#include "Entity.h"
#include "../Level.h"
Position::Position() {
	x = 0;
	y = 0;
}

Position::Position(float x, float y){
	this->x = x;
	this->y = y;
}

Position::~Position()=default;

Position Position::operator+(const Position& p) {
	Position tmp(x, y);
	tmp.x += p.x;
	tmp.y += p.y;
	return tmp;
}

//Entity
Entity::~Entity() = default;

Position Entity::Move(Position pos) {
	this->pos = this->pos + pos;
	return this->pos;
}

Entity::Entity() {
	pos = Position(0, 0);
}
void ReplaceAll(string& c, char f, char t) {
	for (int i = 0; i < c.length(); i++) if (c[i] == f) c[i] = t;
}
Entity::Entity(Position pos, Animator* animator) {
	this->pos = pos;
	this->animator = animator;
}

Car::Car() : Entity() {}

Car::Car(Position pos, Animator* animator) : Entity(pos, animator) {}

void Car::Behavior(int rate) {
	Entity::Behavior(rate);
	Move(Position(1, 0));
	
	//Cham border
	if (pos.x - animator->getWidth() / 2 > LANE_WIDTH) pos.x = -animator->getWidth() / 2;
}

Player::Player() : Entity() {}

Player::Player(Position pos, Animator* animator) : Entity(pos, animator) {}

void Player::Behavior(int rate) {
	Entity::Behavior(rate);

	//Check collision

	//if (pos.x - width / 2 > maxwidth) pos.x = -width / 2;
}