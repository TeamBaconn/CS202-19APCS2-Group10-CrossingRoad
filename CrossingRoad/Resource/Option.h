#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED

#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#define OPTION_OFFSET_X 2

#define PLAY_REQUEST 0
#define LOAD_REQUEST 1
#define CREDIT_REQUEST 2
#define EXIT_REQUEST 3
#define LOAD_SAVE_REQUEST 4
#define BACK_MENU_REQUEST 5
#define SAVE_SAVE_REQUEST 6
#define RESUME_GAME 7

#define ESC 27
#define ENTER 13
#define DELETE_KEY 8

using namespace std;

struct OptionRequest {
	string value;
	int id;
	OptionRequest();
	OptionRequest(int id, string value);
};

class Option {
protected:
	string title;
	string* s;
	int length = 0;
	int select = -1;
public:
	virtual OptionRequest Select(int key);
	string* getOptions();
	int getLength();
	int getSelectIndex();
	string getTitle();
	void Navigate(int k);
	~Option() {
		delete[] s;
	}
};

class MenuOption : public Option {
public:
	MenuOption();
	OptionRequest Select(int key);
};

class LoadOption : public Option {
public:
	LoadOption();
	OptionRequest Select(int key);
};

class CreditOption : public Option {
public:
	CreditOption();
	OptionRequest Select(int key);
};

class LostOption : public Option {
public:
	LostOption(int currentScore, int highScore);
	void saveHighScore(int score);

	OptionRequest Select(int key);
};

class PauseOption : public Option {
private:
	bool saved;
public:
	PauseOption();
	OptionRequest Select(int key);
};

#endif // OPTION_H_INCLUDED