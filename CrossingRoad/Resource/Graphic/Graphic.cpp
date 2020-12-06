#include "Graphic.h"
void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
Graphic::Graphic()  {
	map = new char* [maxheight];
	for (int i = 0; i < maxheight; ++i) {
		map[i] = new char[maxwidth];
		for (int j = 0; j < maxwidth; j++) {
			map[i][j] = ' ';
		}
	}
	for (int i = 0; i < maxheight; i++) {
		for (int j = 0; j < maxwidth; j++) {
			if (i == 0 || j == 0 || i == maxheight - 1 || j == maxwidth - 1) {
				map[i][j] = '#';
			}
		}
	}
}
char** Graphic::getDrawableMap() {
	if (pos == 1) {
		map[5][maxwidth-2] == ' ';
	}
	if (pos == maxwidth - 2) {
		map[5][pos] = 'k';
		map[5][pos - 1] = ' ';
		pos = 1;
	}
	if (pos < maxwidth - 1&&pos>0) {
		map[5][pos] = 'k';
		if(pos-1!=0) map[5][pos - 1] = ' ';
		pos++;
	}
	return map;
}