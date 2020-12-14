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
	Menu() {
		list.push_back(readAnimator("menu_human_face.txt", 1));
		list.push_back(readAnimator("speech_bubble.txt", 1));
		option = new MenuOption();
	}

	void push(int speed) {
		for (int i = 0; i < list.size(); i++) {
			list[i].push(speed);
		}
	}

	vector<AnimatorData>& getList() {
		return list;
	}

	~Menu() {

	}

};

#endif // MENU_H_INCLUDED