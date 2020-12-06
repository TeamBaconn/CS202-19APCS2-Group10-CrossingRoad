#include "GameCore.h"
GameCore::GameCore() {

}
void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void threadFunc(char**newMap) {
	while (1) {
		for (int i = 0; i < maxheight; i++) {
			for (int j = 0; j < maxwidth; j++) {
				GotoXY(j, i); putchar(newMap[i][j]);
			}
			cout << endl;
		}
		Sleep(1000 / 60);
		GotoXY(0, 0);
	}
}
void GameCore::drawGame() {
	char** newMap = graphic.getDrawableMap();
	thread t1(threadFunc,newMap);
	while (1) {
		graphic.getDrawableMap();
	}
	t1.join();

}