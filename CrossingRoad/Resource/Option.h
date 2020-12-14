#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED
#include <string>

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
	OptionRequest() {
		id = -1;
	}
	OptionRequest(int id, string value) {
		this->id = id;
		this->value = value;
	}
};

class Option {
protected:
	string title;
	string* s;
	int length = 0;
	int select = -1;
public:
	virtual OptionRequest Select(int key) { return OptionRequest(); }
	string* getOptions() {
		return s;
	}
	int getLength() {
		return length;
	}
	int getSelectIndex() {
		return select;
	}
	string getTitle() {
		return title;
	}
	void Navigate(int k) {
		select += k;
		if (select >= length) select = 0;
		else if (select < 0) select = length - 1;
	}
};

class MenuOption : public Option{
public:
	MenuOption() {
		title = "I need to take a break, can you help me guide these passengers?";
		length = 4;
		s = new string[length]{"Play","Load game","Credit","Exit"};
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER) return OptionRequest();
		switch (getSelectIndex()) {
		case 0:
			return OptionRequest(PLAY_REQUEST,"");
		case 1:
			return OptionRequest(LOAD_REQUEST, "");
		case 2:
			return OptionRequest(CREDIT_REQUEST, "");
		case 3:
			return OptionRequest(EXIT_REQUEST, "");
		}
		return OptionRequest();
	}
};

class LoadOption : public Option {
public:
	LoadOption() {
		title = "Type in your save's name";
		length = 2;
		s = new string[length]{"", "Back"};
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER && getSelectIndex() == 0) {
			if (key == DELETE_KEY) s[0] = s[0].substr(0, s[0].length()-1);
			else s[0] += (char)key;
			return OptionRequest();
		}
		switch (getSelectIndex()) {
		case 0:
			return OptionRequest(LOAD_SAVE_REQUEST, s[0]);
		case 1:
			return OptionRequest(BACK_MENU_REQUEST, "");
		}
		return OptionRequest();
	}
};

class CreditOption : public Option {
public:
	CreditOption() {
		title = "Here are my creators: ";
		length = 5;
		s = new string[length]{ "Nguyen Lam Tuong - 19125017", 
		"ABC",
		"XYZ",
		"CCD",
		"Back"};
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER) 
			return OptionRequest();
		switch (getSelectIndex()) {
		case 4:
			return OptionRequest(BACK_MENU_REQUEST, "");
		}
		return OptionRequest();
	}
};

class PauseOption : public Option {
public:
	PauseOption() {
		title = "Need to take a break?";
		length = 5;
		s = new string[length]{ "",
		"Save",
		"",
		"Resume",
		"Back to menu"};
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER && getSelectIndex() == 0) {
			if (key == DELETE_KEY) s[0] = s[0].substr(0, s[0].length() - 1);
			else s[0] += (char)key;
			return OptionRequest();
		}
		switch (getSelectIndex()) {
		case 1:
			return OptionRequest(SAVE_SAVE_REQUEST, s[0]);
		case 3:
			return OptionRequest(RESUME_GAME, "");
		case 4:
			return OptionRequest(BACK_MENU_REQUEST, "");
		}
		return OptionRequest();
	}
};

#endif // OPTION_H_INCLUDED