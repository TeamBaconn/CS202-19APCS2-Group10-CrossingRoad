#include "Graphic.h"

void draw(char** map, int x, int y,
	char** pref, int dx, int dy, const Level& level) {
	if (x < 0 || x >= INGAME_WIDTH
		|| y < 0 || y >= INGAME_HEIGHT) return;
	if (dx < 0 || dx >= level.getWidth()
		|| dy < 0 || dy >= level.getHeight()) return;
	if (pref[dx][dy] == '!') return;
	map[x][y] = pref[dx][dy];
}
void drawC(char** map, int x, int y, char c) {
	if (x < 0 || x >= INGAME_WIDTH
		|| y < 0 || y >= INGAME_HEIGHT) return;
	if (c == '!') return;
	map[x][y] = c;
}
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
Graphic::Graphic() = default;

void Graphic::qSort(vector<Entity*>& values, int low, int high) {
	if (low < high) {
		int i = low - 1;
		for (int j = low; j < high; ++j) {
			if (values[j]->pos.y < values[high]->pos.y) {
				++i;
				swap(values[i], values[j]);
			}
		}
		swap(values[i + 1], values[high]);
		qSort(values, low, i);
		qSort(values, i + 2, high);
	}
}

char** Graphic::getDrawableMap(const Level& level, const GameState& state) {
	char** map = Level::reset(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (state == GameState::PLAYING) {
		string score = to_string(level.getScore());
		for (int i = 0; i < score.length(); i++) map[INGAME_WIDTH + 20 + i][10] = score[i];
		//Sort entities
		vector<Entity*> entities = level.getEntities();
		qSort(entities, 0, entities.size() - 1);
		//Draw game scene
		int x = CAM_LOCK_X ? 0 : (int)level.getPlayer()->pos.x - INGAME_WIDTH / 2;
		int y = CAM_LOCK_Y ? 0 : (int)level.getPlayer()->pos.y - INGAME_HEIGHT / 2;
		char** ingame = level.generateMap();
		for (int i = 0; i < INGAME_WIDTH; i++)
			for (int j = 0; j < INGAME_HEIGHT; j++)
				draw(map, i, j, ingame, i + x, j + y, level);


		Level::deleteMap(ingame, level.getWidth());
		for (int i = 0; i < entities.size(); i++) {
			Frame key = entities[i]->data.getFrame();
			Position pos(entities[i]->pos.x - entities[i]->animator->getWidth() / 2
				, entities[i]->pos.y - key.key.size());
			for (int j = 0; j < key.key.size(); j++) {
				for (int k = 0; k < key.key[j].length(); k++) {
					//Check its null character ! by default
					char c = key.key[j][k];
					drawC(map, pos.x + k - x, pos.y + j - y, c);
				}
			}
			//For debug
			drawC(map, entities[i]->pos.x - x, entities[i]->pos.y - y, '0' + i);
			drawC(map, pos.x - x, pos.y - y, 'X');
		}
	}
	return map;
}