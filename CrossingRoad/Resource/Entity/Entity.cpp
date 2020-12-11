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
Entity::Entity(Position pos, Animator * animator) {
	this->pos = pos;
	this->animator = animator;
	this->data = AnimatorData(animator);
}

Car::Car() : Entity() {}

Car::Car(Position pos, Animator * animator) : Entity(pos, animator) {}

bool Car::Behavior(int rate, Level & level) {
	Entity::Behavior(rate, level);
	Move(Position(1, 0));
	int firstHalf = GetPos().x + getAni()->getWidth() / 2;
	int secondHalf = GetPos().x - getAni()->getWidth() / 2;
	int botY = GetPos().y;
	int topY = getAni()->getHeight();
	if (secondHalf < level.getPlayer()->GetPos().x
		&& level.getPlayer()->GetPos().x < firstHalf
		&& botY>level.getPlayer()->GetPos().y
		&& level.getPlayer()->GetPos().y > botY - topY) {
		level.LooseGame();
	}
	return pos.x - animator->getWidth() / 2 > LANE_WIDTH;
}

bool Car::isCar()
{
	return true;
}

Player::Player() : Entity() {}

Player::Player(Position pos, Animator * animator) : Entity(pos, animator) {}


bool Player::Behavior(int rate, Level & lvl) {
	Entity::Behavior(rate, lvl);
	if (pos.y > lvl.getHeight()) {
		++lvl.getMode();
		pos.x = LANE_WIDTH / 2;
		pos.y = 3;
		return true;
	}
	return false;
	//if (pos.x - width / 2 > maxwidth) pos.x = -width / 2;
}

Prop::Prop() : Entity() {}

Prop::Prop(Position pos, Animator * animator) : Entity(pos, animator) {}