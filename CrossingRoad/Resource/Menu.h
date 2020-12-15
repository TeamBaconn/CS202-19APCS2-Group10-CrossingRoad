#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Graphic/Animator.h"
#include "Option.h"

class Menu {
private:
	vector<AnimatorData> list;
public:
	Option* option;
	//Constructor
	Menu();

	void push(int speed);

	vector<AnimatorData>& getList();

	~Menu() {

	}

};

#endif // MENU_H_INCLUDED