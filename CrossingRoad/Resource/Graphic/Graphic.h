#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED
#define maxwidth 210
#define maxheight 45
#include <Windows.h>
using namespace std;
class Graphic {
private:
	char** map;
	int pos = 1;
public:
	//Constructor
	Graphic();

	~Graphic() {};
	
	char** getDrawableMap();
};
void GotoXY(int x, int y);
#endif // GRAPHIC_H_INCLUDED