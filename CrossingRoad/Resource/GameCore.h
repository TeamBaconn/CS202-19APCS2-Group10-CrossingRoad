#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED

#include <iostream>
#include <vector>
#include "Graphic/Graphic.h"
#include "Entity/Entity.h"

using namespace std;

class GameCore {
private:
	vector<Entity*> entities;
	Graphic graphic;

public:
	GameCore();
	~GameCore();

	void drawGame(); //Using graphic

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

#endif // GAMECORE_H_INCLUDED