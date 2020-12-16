#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Graphic/Animator.h"
#include "Option.h"

class Menu {
private:
	vector<AnimatorData> list;
	Option* option;
public:
	//Constructor
	Menu();

	void push(int speed);

	vector<AnimatorData>& getList();

	void MenuChange(Option* op);

	~Menu();
	friend class GameCore;
	friend class Graphic;
};

#endif // MENU_H_INCLUDED