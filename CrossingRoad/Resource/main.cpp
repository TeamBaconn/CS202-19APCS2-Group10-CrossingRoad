#include "GameCore.h"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	FixConsoleWindow();
	HideCursor();

	GameCore gc;
	gc.Start();
	return 0;
}