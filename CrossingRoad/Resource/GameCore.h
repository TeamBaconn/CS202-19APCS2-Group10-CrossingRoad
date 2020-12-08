#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED
#include "chrono"
#include <iostream>
#include <fstream>
#include <vector>
#include "Graphic/Graphic.h"
#include <conio.h>
#include <thread>

#define GAME_RATE 50

using namespace std;

class GameCore {
private:
	Graphic graphic;
	Level level;
public:
	GameCore();
	~GameCore() {};

	void drawGame(); //Using graphic
	void GameBehavior();
	void UserInput();
	

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