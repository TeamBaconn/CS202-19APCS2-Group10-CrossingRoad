#include "Entity.h"

Position::Position() {
	x = 0;
	y = 0;
}

Position::Position(int x, int y){
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
	pos = *new Position();
}

Entity::Entity(Position pos) {
	this->pos = pos;
}