#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED

#include "chrono"
#include <iostream>
#include <fstream>
#include <vector>

#include "Graphic/Graphic.h"

#include <conio.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>

#pragma comment(lib,"Winmm.lib")
using namespace std;

class GameCore {
private:
	GameState state;
	Graphic graphic;
	Level level;
	Menu menu;
public:
	GameCore();
	~GameCore();

	void DrawGame(); //Using graphic
	void GameBehavior();
	void UserInput();
	void HandleInput(int key);
	
	void Start();

	friend class Graphic;
};
void FixConsoleWindow();
void resizeConsole(int width, int height);
void HideCursor();
#endif // GAMECORE_H_INCLUDED