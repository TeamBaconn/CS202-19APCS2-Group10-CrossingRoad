#include "Menu.h"

Menu::Menu()
{
	string nothing = "";
	list.push_back(readAnimator("menu_human_face.txt",nothing, 1));
	list.push_back(readAnimator("speech_bubble.txt",nothing, 1));
	option = new MenuOption();
}

void Menu::push(int speed)
{
	for (int i = 0; i < list.size(); i++) {
		list[i].push(speed);
	}
}

vector<AnimatorData>& Menu::getList()
{
	return list;
}
