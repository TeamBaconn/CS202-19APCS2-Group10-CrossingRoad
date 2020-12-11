#include "GameCore.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return is_base_of<Base, T>::value;
}

GameCore::GameCore() {
	this->state = GameState::MENU;
	//Load du lieu nguoi choi
	//level = Level(10,1);
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
		menu.push(GAME_RATE);
		if (state != GameState::PLAYING) continue;
		if (level.isLost()) {
			state = GameState::MENU;
		}
		level.CheckEntity();
	}
}
void GameCore::UserInput() {
	while (1) {
		Sleep(50);
		char c = _getch();

		if (state != GameState::PLAYING) {
			level = Level(2,1);
			state = GameState::PLAYING;
			continue;
		}
		Entity* player = level.player;
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
			player->Move(Position(-2,0));
			player->changeBase(2);
			break;
		case 77:
			player->Move(Position(2,0));
			player->changeBase(1);
			break;
		}
	}
}
void GameCore::Start() {
	//Draw game
	thread t3(&GameCore::DrawGame, this);
	//Behavior
	thread t1(&GameCore::GameBehavior, this);
	//Nhan input tu user
	thread t2(&GameCore::UserInput, this);
	t1.join();
	t2.join();
	t3.join();
}
void GameCore::DrawGame() {
	char** old = nullptr;
	while (1) {
		char** map = graphic.getDrawableMap(level,state);

		graphic.drawMenu(map, menu);

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
}