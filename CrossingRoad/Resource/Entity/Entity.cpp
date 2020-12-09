#include "Entity.h"
#include "../Level.h"
Position::Position() {
	x = 0;
	y = 0;
}

Position::Position(float x, float y) {
	this->x = x;
	this->y = y;
}

Position::~Position() = default;

Position Position::operator+(const Position & p) {
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
void ReplaceAll(string & c, char f, char t) {
	for (int i = 0; i < c.length(); i++) if (c[i] == f) c[i] = t;
}
Entity::Entity(Position pos, Animator * animator) {
	this->pos = pos;
	this->animator = animator;
}

Car::Car() : Entity() {}

Car::Car(Position pos, Animator * animator) : Entity(pos, animator) {}

bool Car::Behavior(int rate, Level & level) {
	Entity::Behavior(rate, level);
	Move(Position(1, 0));

	return pos.x - animator->getWidth() / 2 > LANE_WIDTH;
}

Player::Player() : Entity() {}

Player::Player(Position pos, Animator * animator) : Entity(pos, animator) {}


bool Player::Behavior(int rate, Level & lvl) {
	Entity::Behavior(rate, lvl);
	for (int i = 0; i < lvl.getEntities().size(); i++) {
		if (lvl.getEntities()[i] == this) continue;
		int firstHalf = lvl.getEntities()[i]->GetPos().x + lvl.getEntities()[i]->getAni()->getWidth() / 2;
		int secondHalf = lvl.getEntities()[i]->GetPos().x - lvl.getEntities()[i]->getAni()->getWidth() / 2;
		int botY = lvl.getEntities()[i]->GetPos().y;
		int topY = lvl.getEntities()[i]->getAni()->getHeight();
		if (secondHalf < pos.x && pos.x < firstHalf && botY>pos.y&&pos.y> botY-topY) {
			lvl.LooseGame();
		}
	}
	return false;
	//if (pos.x - width / 2 > maxwidth) pos.x = -width / 2;
}