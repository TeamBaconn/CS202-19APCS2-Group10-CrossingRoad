#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Entity/Entity.h"

class Menu {
private:
	vector<AnimatorData> list;
public:
	//Constructor
	Menu() {
		readAnimator("", 1);
	}

	void push(int speed) {
		for (int i = 0; i < list.size(); i++) list[i].push(speed);
	}

	vector<AnimatorData> getData() {
		return list;
	}

	~Menu() {

	}

};

#endif // MENU_H_INCLUDED