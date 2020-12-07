#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <Windows.h>
#include "../Entity/Entity.h"
#include <vector>

using namespace std;
class Graphic {
private:

public:
	//Constructor
	Graphic();

	~Graphic() {};
	
	vector<Entity*> sort(vector<Entity*> values);
	char** getDrawableMap(const vector<Entity*>& en);
};
void GotoXY(int x, int y);
#endif // GRAPHIC_H_INCLUDED