#include "Graphic.h"
#include <iostream>
void draw(char**& map, int x, int y, char c) {
	if (x < 0 || x >= maxwidth || y < 0 || y >= maxheight) return;
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

char** Graphic::getDrawableMap(const vector<Entity*>& e) {
	vector<Entity*> entities = sort(e);
	char** map = new char* [maxwidth];
	for (int i = 0; i < maxwidth; ++i) {
		map[i] = new char[maxheight];
		for (int j = 0; j < maxheight; j++) {
			map[i][j] = ' ';
		}
	}
	for (int i = 0; i < maxwidth; i++) {
		for (int j = 0; j < maxheight; j++) {
			if (i == 0 || j == 0 || i == maxwidth - 1 || j == maxheight - 1) {
				map[i][j] = '#';
			}
		}
	}

	for (int i = 0; i < entities.size(); i++) {
		Position pos(entities[i]->pos.x - entities[i]->width / 2
			, entities[i]->pos.y - entities[i]->animator.size());
		for (int j = 0; j < entities[i]->animator.size(); j++) {
			for (int k = 0; k < entities[i]->animator[j].length(); k++) {
				//Check its null character ! by default
				if (entities[i]->animator[j][k] == '!') continue;

				draw(map, pos.x + k, pos.y + j, entities[i]->animator[j][k]);

			}
		}
		draw(map, entities[i]->pos.x, entities[i]->pos.y, '0' + i);
		draw(map, pos.x, pos.y, 'X');
	}
	return map;
}
