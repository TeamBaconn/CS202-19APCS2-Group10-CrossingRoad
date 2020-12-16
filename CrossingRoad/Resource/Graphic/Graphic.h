#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <Windows.h>
#include <vector>

#include "../Level.h"
#include "../Menu.h"

using namespace std;
class Graphic {
private:

public:
	//Constructor
	Graphic();

	~Graphic();

	void drawMenu(char** map, Menu& menu, const GameState& state);
	char** getDrawableMap(const Level& level, const GameState& state);
};
void GotoXY(int x, int y);
#endif // GRAPHIC_H_INCLUDED