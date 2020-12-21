#include "GameCore.h"
#include <cassert>

GameCore::GameCore() {
	this->state = GameState::MENU;
}

GameCore::~GameCore()
{
}

void GameCore::GameBehavior() {
	while (1) {
		Sleep(GAME_RATE);
		menu.push(GAME_RATE);
		if (state != GameState::PLAYING) continue;
		level.lost += (level.lost ? GAME_RATE : 0);
		if (level.lost > LOST_DELAY) {
			//Return to menu

			menu.MenuChange(new LostOption(level.score, level.highscore));
			state = GameState::MENU;
			level.resetLevel();
			continue;
		}
		level.CheckEntity();
	}
}
void GameCore::HandleInput(int c) {
	switch (c) {
	case 72:
		menu.option->Navigate(-1); 
		Level::PlaySoundEffect("Button_Navigate.wav");
		return;
	case 80:
		menu.option->Navigate(1);
		Level::PlaySoundEffect("Button_Navigate.wav");
		return;
	case ENTER:
		Level::PlaySoundEffect("Button_Click.wav");
		break;
	case -32:
		return;
	}
	OptionRequest op = menu.option->Select(c);
	switch (op.id) {
	case LOAD_SAVE_REQUEST:
		//Load save o day nay
		if(op.value.length() == 0) break;
		level.resetLevel();
		level.loadLevel(op.value);
		state = GameState::PLAYING;
		break;
	case SAVE_SAVE_REQUEST:
		//Save save o day nay
		if (op.value.length() == 0) break;
		level.saveLevel(op.value);
		break;
	case PLAY_REQUEST:
		level.resetLevel();
		state = GameState::PLAYING;
		break;
	case LOAD_REQUEST:
		menu.MenuChange(new LoadOption());
		break;
	case BACK_MENU_REQUEST:
		menu.MenuChange(new MenuOption());
		break;
	case CREDIT_REQUEST:
		menu.MenuChange(new CreditOption());
		break;
	case RESUME_GAME:
		state = GameState::PLAYING;
		break;
	case EXIT_REQUEST:
		exit(0);
	}
}
void GameCore::UserInput() {
	while (1) {
		Sleep(GAME_RATE);
		char c = _getch();

		if (state != GameState::PLAYING) {
			HandleInput(c);
			continue;
		}
		if (level.lost) continue;
		Entity* player = level.player;
		switch ((int)c) {
		case 80:
			player->Move(Position(0, 1));
			player->changeBase(3);
			break;
		case 72:
			player->Move(Position(0, -1));
			player->changeBase(3);
			break;
		case 75:
			player->Move(Position(-2, 0));
			player->changeBase(2);
			break;
		case 77:
			player->Move(Position(2, 0));
			player->changeBase(1);
			break;
		case ESC:
			this->state = GameState::PAUSE;
			menu.MenuChange(new PauseOption());
		}
	}
}

void GameCore::Start() {
	//Draw game
	thread t3(&GameCore::DrawGame, this);
	//Behavior
	thread t1(&GameCore::GameBehavior, this);
	//Nhan input tu user
	thread t2(&GameCore::UserInput, this);;
	//for bomb sound
	t1.join();
	t2.join();
	t3.join();
}
void set_console_size(HANDLE& screen_buffer, SHORT width, SHORT height)
{
	assert(screen_buffer != NULL);
	assert(width > 0);
	assert(height > 0);

	COORD const size = { width, height };
	BOOL success;

	SMALL_RECT const minimal_window = { 0, 0, 1, 1 };
	success = SetConsoleWindowInfo(screen_buffer, TRUE, &minimal_window);

	success = SetConsoleScreenBufferSize(screen_buffer, size);

	SMALL_RECT const window = { 0, 0, size.X - 1, size.Y - 1 };
	success = SetConsoleWindowInfo(screen_buffer, TRUE, &window);
}
void GameCore::DrawGame() {
	//Setup window
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD dwBytesWritten = 0;

	set_console_size(hConsole, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetConsoleActiveScreenBuffer(hConsole);

	SetConsoleTitle("Crossing Road | CS202");
	WORD* screen_color = new WORD[SCREEN_WIDTH * SCREEN_HEIGHT];
	char* screen = new char[SCREEN_WIDTH * SCREEN_HEIGHT];

	while (1) {

		char** map = graphic.getDrawableMap(level, state);
		
		graphic.drawMenu(map, menu, state);

		for (int i = 0; i < SCREEN_HEIGHT; i++) for (int j = 0; j < SCREEN_WIDTH; j++) {
			screen[i * SCREEN_WIDTH + j] = map[j][i];
			if (state == GameState::PLAYING && (map[j][i] == 'G' || map[j][i] == 'R')) {
				screen[i * SCREEN_WIDTH + j] = ' ';
				screen_color[i * SCREEN_WIDTH + j] = map[j][i] == 'G' ? BACKGROUND_GREEN: BACKGROUND_RED;
			}
			else screen_color[i * SCREEN_WIDTH + j] = 7;
		}
		WriteConsoleOutputCharacter(hConsole, (LPCSTR)	screen, SCREEN_WIDTH * SCREEN_HEIGHT, {0,0}, &dwBytesWritten);
		WriteConsoleOutputAttribute(hConsole, screen_color, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);
		Level::deleteMap(map, SCREEN_WIDTH);
	}
}