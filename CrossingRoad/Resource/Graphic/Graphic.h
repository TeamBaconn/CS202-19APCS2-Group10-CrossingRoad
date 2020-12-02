#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

class Graphic {
private:
	int** map;
public:
	//Constructor
	Graphic();

	~Graphic();
	
	int** getDrawableMap();
};

#endif // GRAPHIC_H_INCLUDED