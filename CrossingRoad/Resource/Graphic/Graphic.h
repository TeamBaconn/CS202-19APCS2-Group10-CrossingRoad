#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <Windows.h>
#include <vector>

#include "../Level.h"

using namespace std;
class Graphic {
private:

public:
	//Constructor
	Graphic();

	~Graphic() {};
	
	void qSort(vector<Entity*> values, int low, int high);
	vector<Entity*> sort(vector<Entity*> values);
	char** getDrawableMap(const Level& level, const GameState& state);
};
void GotoXY(int x, int y);
#endif // GRAPHIC_H_INCLUDED