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
	pos = Position(0, 0);
}
void ReplaceAll(string& c, char f, char t) {
	for (int i = 0; i < c.length(); i++) if (c[i] == f) c[i] = t;
}
Entity::Entity(Position pos, string type) {
	this->pos = pos;
	ifstream f;
	f.open("./Resource/Animation/" + type);
	string c;
	while (!f.eof()) {
		getline(f, c);
		cout << c << endl;
		ReplaceAll(c, ' ', '!');
		animator.push_back(c);
		if (width == -1 || width < c.length()) width = c.length();
	}
	cout << width << endl;
	f.close();
}

Car::Car() : Entity() {}

Car::Car(Position pos, string type) : Entity(pos, type) {}

void Car::Behavior() {
	Move(Position(1, 0));
	if (pos.x - width / 2 > maxwidth) pos.x = -width/2;
}

Player::Player() : Entity() {}

Player::Player(Position pos, string type) : Entity(pos, type) {}

void Player::Behavior() {
	if (pos.x - width / 2 > maxwidth) pos.x = -width / 2;
}