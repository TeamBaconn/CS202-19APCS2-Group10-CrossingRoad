#include "GameCore.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return is_base_of<Base, T>::value;
}

GameCore::GameCore() {
	ifstream info;
	info.open(ANIMATION_SETTING);
	int k,type_id;
	info >> k;
	string type;
	for (int i = 0; i < k; i++) {
		Entity* entity;
		info >> type_id;
		info.ignore();
		getline(info, type);
		switch (type_id) {
		case 1:
			entity = new Car(Position(10,10*i),type);
			break;
		case 0:
			entity = new Player(Position(20, 20), type);
			break;
		}
		entities.push_back(entity);
	}
}
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void GameCore::GameBehavior() {
	while (1) {
		Sleep(30);
		for(int i = 0; i < entities.size(); i++) 
			entities[i]->Behavior();
	}
}
void GameCore::UserInput() {
	Entity* player = entities[0];
	while (1) {
		char c = _getch();
		/*
		* 75 <-
		* 80 v
		* 77 ->
		* 72 ^
		*/
		switch ((int)c) {
		case 80:
			player->Move(Position(0,1));
			break;
		case 72:
			player->Move(Position(0,-1));
			break;
		case 75:
			player->Move(Position(-1,0));
			break;
		case 77:
			player->Move(Position(1,0));
			break;
		}
		
	}
}
void GameCore::drawGame() {
	thread t1(&GameCore::GameBehavior,this);
	thread t2(&GameCore::UserInput, this);
	char** old = nullptr;
	while (1) {
		char** map = graphic.getDrawableMap(entities);
		for (int i = 0; i < maxwidth; i++) {
			for (int j = 0; j < maxheight; j++) {
				if (old != nullptr && old[i][j] == map[i][j]) continue;
				GotoXY(i, j);
				putchar(map[i][j]);
			}
		}
		//Delete old map
		if (old != nullptr) {
			for (int i = 0; i < maxwidth; i++) delete old[i];
			delete old;
		}
		old = map;
	}
	t1.join();
	t2.join();
}