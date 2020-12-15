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

void Position::operator=(const Position & p) {
	this->x = p.x;
	this->y = p.y;
}

//Entity
Entity::~Entity() = default;

Position Entity::Move(Position pos) {
	this->pos = this->pos + pos;
	return this->pos;
}

void Entity::changeBase(int id) {
	data.changeAnimation(id);
}

Position Entity::GetPosition() const
{
	return pos;
}

Animator* Entity::getAnimator()
{
	return animator;
}

AnimatorData& Entity::getAnimatorData() {
	return data;
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

Car::Car(Position pos, Animator* animator, int lane) : Entity(pos, animator) {
	this->lane = lane;
}

bool Car::Behavior(int rate, Level & level) {
	Entity::Behavior(rate, level);
	data.reverse = !level.getLane(lane).toRight;
	if (level.getLane(lane).open) {
		if (!data.reverse) Move(Position(1, 0));
		else Move(Position(-1, 0));
	}
	int firstHalf = GetPosition().x + getAnimatorData().getWidth() / 2;
	int secondHalf = GetPosition().x - getAnimatorData().getWidth() / 2;
	int botY = GetPosition().y;
	int topY = GetPosition().y-getAnimatorData().getHeight();
	int vertical = GetPosition().y - level.getPlayer()->GetPosition().y;
	if (level.getPlayer()->GetPosition().x>=GetPosition().x-9&& level.getPlayer()->GetPosition().x <= GetPosition().x + 9) {
		if(vertical>0&&vertical<8)
		level.setSoundName(getAnimator()->getSound());
	}
	if (secondHalf <= level.getPlayer()->GetPosition().x
		&& level.getPlayer()->GetPosition().x <= firstHalf
		&& botY>=level.getPlayer()->GetPosition().y
		&& level.getPlayer()->GetPosition().y >= (botY + topY)/2) {
		level.LooseGame();
		remove = true;
	}
	if (!data.reverse) return pos.x - animator->getWidth() / 2 > LANE_WIDTH;
	else return pos.x + animator->getWidth() / 2 < 0; 
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
		pos.x = LANE_WIDTH / 2;
		pos.y = 3;
		return true;
	}
	return false;
}

Position Player::Move(Position pos) {
	Position tmp(this->pos + pos);
	if (tmp.x-getAnimator()->getWidth()/2 < 0 
		|| tmp.y < 0 
		|| tmp.x + getAnimator()->getWidth() / 2 > LANE_WIDTH) return this->pos;
	return Entity::Move(pos);
}

Prop::Prop() : Entity() {}

Prop::Prop(Position pos, Animator * animator) : Entity(pos, animator) {}

Light::Light() = default;

Light::Light(Position pos, Animator* animator, int lane) : Entity(pos, animator), lane(lane) {}

bool Light::isCar() {
	return true;
}

bool Light::Behavior(int rate, Level& lvl) {
	Entity::Behavior(rate, lvl);
	data.changeAnimation((!lvl.getLane(lane).open));
	return false;
}