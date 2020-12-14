#include "Graphic.h"

void drawC(char** map, int x, int y, char c, bool InGame) {
	if (InGame) {
		if (x < 0 || x >= INGAME_WIDTH
			|| y < 0 || y >= INGAME_HEIGHT) return;
	}
	else if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
	if (c == '!') return;
	map[x][y] = c;
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
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int DrawString(char** map, const string& c, int x, int y, int dx) {
	int rx = 0;
	for (int i = 0; i < c.length(); i++) {
		if (rx == 0 && c[i] == ' ') continue;
		drawC(map, rx+x, y, c[i], false);
		rx++;
		if (rx > dx) {
			rx = 0;
			y++;
		}
	}
	return y;
}

//Constructor
Graphic::Graphic() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	FixConsoleWindow();
	HideCursor();
}

void qSort(vector<Entity*>& values, int low, int high) {
	if (low < high) {
		int i = low - 1;
		for (int j = low; j < high; ++j) {
			if (values[j]->GetPosition().y - values[j]->getAnimator()->getOffSetY() 
				< values[high]->GetPosition().y - values[high]->getAnimator()->getOffSetY()) {
				++i;
				swap(values[i], values[j]);
			}
		}
		swap(values[i + 1], values[high]);
		qSort(values, low, i);
		qSort(values, i + 2, high);
	}
}

void Graphic::drawMenu(char** map, Menu& menu, const GameState& state) {
	vector<AnimatorData>& data = menu.getList();
	data[0].reverse = state == GameState::PLAYING;
	Frame frame = data[0].getFrame();
	for (int i = 0; i < frame.key.size(); i++)
		for (int j = 0; j < frame.key[i].length(); j++) 
			if(frame.key[i][j] != '!')
			map[j+INGAME_WIDTH+5][i+9] = frame.key[i][j];
	
	if (state == GameState::PLAYING) return;
	frame = data[1].getFrame();
	for (int i = 0; i < frame.key.size(); i++)
		for (int j = 0; j < frame.key[i].length(); j++)
			if (frame.key[i][j] != '!')
				map[40+j][i+5] = frame.key[i][j];

	int max_y = 7;
	max_y = DrawString(map, menu.option->getTitle(), 44, max_y, 80) + 1;
	for (int i = 0; i < menu.option->getLength(); i++) {
		max_y = DrawString(map, 
			(menu.option->getSelectIndex() == i ? ">> " : "") + menu.option->getOptions()[i]
			, 44+OPTION_OFFSET_X, max_y+1, 80);
	}
}

char** Graphic::getDrawableMap(const Level& level, const GameState& state) {
	char** map = Level::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (state == GameState::PLAYING) {
		int width = level.getWidth(), height = level.getHeight();

		string score = "Score: "+to_string(level.getScore());
		DrawString(map, score, INGAME_WIDTH + 5, 6, 20);
		//Sort entities
		vector<Entity*> entities = level.getEntities();
		qSort(entities, 0, entities.size() - 1);
		//Draw game scene
		int x = CAM_LOCK_X ? 0 : (int)level.getPlayer()->GetPosition().x - INGAME_WIDTH / 2;
		int y = CAM_LOCK_Y ? 0 : (int)level.getPlayer()->GetPosition().y - INGAME_HEIGHT / 2;

		for (int i = 1; i <= level.getLane(); i++)
			for (int j = 0; j < width; j += LANE_DISTANCE) drawC(map, j - x, i*LANE_HEIGHT - y, '-', true); 
			
		for (int i = 0; i < entities.size(); i++) {
			Frame key = entities[i]->getAnimatorData().getFrame();
			Position pos(entities[i]->GetPosition().x - key.key[0].length() / 2
				, entities[i]->GetPosition().y - key.key.size());
			for (int j = 0; j < key.key.size(); j++) {
				for (int k = 0; k < key.key[j].length(); k++) {
					//Check its null character ! by default
					char c = key.key[j][k];
					drawC(map, pos.x + k - x, pos.y + j - y, c,true);
				}
			}

			//Frame the game scene
			for (int i = 0; i < width; i++)
				for (int j = 0; j < height; j++)
					if (i == 0 || i == width - 1)
						drawC(map, i - x, j - y, '|',true);

			//For debug
			//drawC(map, entities[i]->GetPosition().x - x, entities[i]->GetPosition().y - y, '0' + i);
			//drawC(map, pos.x - x, pos.y - y, 'X');
		}
	}
	return map;
}