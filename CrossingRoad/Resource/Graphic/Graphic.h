#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <Windows.h>
#include <vector>

#include "../Level.h"
#include "../Menu.h"

using namespace std;
class Graphic {
public:
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//Constructor
	Graphic();

	~Graphic();

	void drawMenu(char** map, Menu& menu, const GameState& state);
	char** getDrawableMap(const Level& level, const GameState& state);
};
#endif // GRAPHIC_H_INCLUDED