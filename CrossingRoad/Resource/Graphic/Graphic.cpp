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
Graphic::Graphic() {

}
vector<Entity*> Graphic::sort(vector<Entity*> values) {
	for(int i = 0; i < values.size()-1; i++)
		for (int j = i + 1; j < values.size(); j++)if (values[i]->pos.y > values[j]->pos.y) {
			Entity* tmp = values[i];
			values[i] = values[j];
			values[j] = tmp;
		}
	return values;
}

char** Graphic::getDrawableMap(const Level& level) {

	vector<Entity*> entities = sort(level.getEntities());

	char** map = Level::reset(SCREEN_WIDTH, SCREEN_HEIGHT);
	int x = CAM_LOCK_X ? 0 : (int)level.getPlayer()->pos.x - INGAME_WIDTH / 2;
	int y = CAM_LOCK_Y ? 0 : (int)level.getPlayer()->pos.y - INGAME_HEIGHT / 2;
	//Draw game scene
	char** ingame = level.generateMap();
	for (int i = 0; i < INGAME_WIDTH; i++)
		for (int j = 0; j < INGAME_HEIGHT; j++)
			draw(map,i,j,ingame,i+x,j + y,level);

	Level::deleteMap(ingame, level.getWidth());
	for (int i = 0; i < entities.size(); i++) {
		Position pos(entities[i]->pos.x - entities[i]->animator->getWidth() / 2
			, entities[i]->pos.y - entities[i]->animator->getKey().key.size());
		for (int j = 0; j < entities[i]->animator->getKey().key.size(); j++) {
			for (int k = 0; k < entities[i]->animator->getKey().key[j].length(); k++) {
				//Check its null character ! by default
				char c = entities[i]->animator->getKey().key[j][k];

				drawC(map, pos.x + k-x, pos.y + j-y, c);

			}
		}
		//For debug
		//drawC(map, entities[i]->pos.x -x, entities[i]->pos.y-y, '0' + i);
		//drawC(map, pos.x-x, pos.y-y, 'X');
	}
	return map;
}
