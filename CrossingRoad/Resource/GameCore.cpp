#include "GameCore.h"

GameCore::GameCore() {
	this->state = GameState::MENU;
}

void GameCore::GameBehavior() {
	while (1) {
		Sleep(GAME_RATE);
		menu.push(GAME_RATE);
		if (state != GameState::PLAYING) continue;
		level.lost += (level.lost ? GAME_RATE : 0);
		if (level.lost > LOST_DELAY) {
			//Return to menu

			ifstream fin("./Resource/Data/highscore.txt");
			int highscore = level.score;
			if (fin) {
				fin >> highscore >> highscore;
				highscore = highscore > level.score ? highscore : level.score;
				fin.close();
			}

			menu.ChangeMenu(new LostOption(level.score, highscore));
			state = GameState::MENU;
			level = Level();
			continue;
		}
		level.CheckEntity();
	}
}
void GameCore::HandleInput(int c) {
	switch (c) {
	case 72:
		menu.option->Navigate(-1);
		return;
	case 80:
		menu.option->Navigate(1);
		return;
	case -32:
		return;
	}
	OptionRequest op = menu.option->Select(c);
	switch (op.id) {
	case LOAD_SAVE_REQUEST:
		//Load save o day nay
		level = Level();
		level.loadLevel(op.value);
		state = GameState::PLAYING;
		break;
	case SAVE_SAVE_REQUEST:
		//Save save o day nay
		level.saveLevel(op.value);
		break;
	case PLAY_REQUEST:
		level = Level();
		state = GameState::PLAYING;
		break;
	case LOAD_REQUEST:
		menu.ChangeMenu(new LoadOption());
		break;
	case BACK_MENU_REQUEST:
		menu.ChangeMenu(new MenuOption());
		level = Level();
		break;
	case CREDIT_REQUEST:
		menu.ChangeMenu(new CreditOption());
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
			menu.ChangeMenu(new PauseOption());
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
void SetColor(int ForgC)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}
void GameCore::DrawGame() {
	char** old = nullptr;
	while (1) {
		char** map = graphic.getDrawableMap(level, state);

		graphic.drawMenu(map, menu, state);

		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				if (old != nullptr) {
					if (old[i][j] == map[i][j]) continue;
				}
				else if (map[i][j] == ' ') continue;
				GotoXY(i, j);
				if (map[i][j] == 'G' && state == GameState::PLAYING) {
					SetColor(10);
					putchar((char)220);
				}
				else if (map[i][j] == 'R' && state == GameState::PLAYING) {
					SetColor(4);
					putchar((char)220);
				}
				else {
					SetColor(15);
					putchar(map[i][j]);
				}
			}
		}
		//Delete old map
		Level::deleteMap(old, SCREEN_WIDTH);
		old = map;
	}
}