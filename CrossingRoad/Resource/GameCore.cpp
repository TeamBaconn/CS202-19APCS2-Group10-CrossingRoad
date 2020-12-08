#include "GameCore.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return is_base_of<Base, T>::value;
}

GameCore::GameCore() {
	level = Level(10,1);
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
		Sleep(GAME_RATE);
		for(int i = 0; i < level.getEntities().size(); i++) 
			level.getEntities()[i]->Behavior(GAME_RATE);
	}
}
void GameCore::UserInput() {
	Entity* player = level.player;
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
			player->changeBase(3);
			break;
		case 72:
			player->Move(Position(0,-1));
			player->changeBase(3);
			break;
		case 75:
			player->Move(Position(-1,0));
			player->changeBase(2);
			break;
		case 77:
			player->Move(Position(1,0));
			player->changeBase(1);
			break;
		}
		Sleep(50);
	}
}
void GameCore::drawGame() {
	//Behavior
	thread t1(&GameCore::GameBehavior,this);
	//Nhan input tu user
	thread t2(&GameCore::UserInput, this);

	//Draw
	char** old = nullptr;
	while (1) {
		char** map = graphic.getDrawableMap(level);

		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				if (old != nullptr && old[i][j] == map[i][j]) continue;
				GotoXY(i, j);
				putchar(map[i][j]);
			}
		}
		//Delete old map
		Level::deleteMap(old, SCREEN_WIDTH);
		old = map;
	}
	t1.join();
	t2.join();
}