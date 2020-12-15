#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED
#define SOUND "./Resource/Sound/"
#include "chrono"
#include <iostream>
#include <fstream>
#include <vector>

#include "Graphic/Graphic.h"

#include <conio.h>
#include <stdlib.h>
#include <thread>
#include <windows.h>
#include <mmsystem.h>
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
	~GameCore() {};

	void DrawGame(); //Using graphic
	void GameBehavior();
	void UserInput();
	void HandleInput(int key);
	void SoundBehavior();
	
	void Start();

	friend class Graphic;
	/*
	void resetGame(); 
	void exitGame(HANDLE);
	void startGame(); 
	void loadGame(istream);
	void saveGame(istream); 
	void pauseGame(HANDLE); 
	void resumeGame(HANDLE); 
	void updatePosPeople(char); 
	void updatePosVehicle(); 
	void updatePosAnimal();
	*/
};
void FixConsoleWindow();
void resizeConsole(int width, int height);
void HideCursor();
#endif // GAMECORE_H_INCLUDED