#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED

#include <string>
#include <algorithm>

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

class MenuOption : public Option {
public:
	MenuOption() {
		title = "I need to take a break, can you help me guide these passengers?";
		length = 4;
		s = new string[length]{ "Play","Load game","Credit","Exit" };
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER) return OptionRequest();
		switch (getSelectIndex()) {
		case 0:
			return OptionRequest(PLAY_REQUEST, "");
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
		s = new string[length]{ "", "Back" };
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER && getSelectIndex() == 0) {
			if (key == DELETE_KEY) s[0] = s[0].substr(0, s[0].length() - 1);
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
		length = 6;
		s = new string[length]{ "Nguyen Lam Tuong - 19125017",
		"Truong Quy Hai - 19125090",
		"XYZ",
		"CCD",
		"",
		"Back" };
		select = 0;
	}
	OptionRequest Select(int key) {
		if (key != ENTER)
			return OptionRequest();
		switch (getSelectIndex()) {
		case 5:
			return OptionRequest(BACK_MENU_REQUEST, "");
		}
		return OptionRequest();
	}
};

class LostOption : public Option {
public:
	LostOption(int currentScore, int highScore) {
		saveHighScore(currentScore);
		title = "Useless. Guess I have to do it myself >:(";
		length = 4;
		s = new string[length]{ "You scored: " + to_string(currentScore),
			"High score: " + to_string(highScore),
			"",
		"Back to menu" };
		select = 0;
	}
	void saveHighScore(int score) {
		ifstream fin("./Resource/Data/highscore.txt");
		if (fin) {
			int count;
			fin >> count;
			vector<int> scores;
			int s;
			for (int i = 0; i < count; ++i) {
				fin >> s;
				scores.push_back(s);
			}
			scores.push_back(score);
			sort(scores.begin(), scores.end(), [](int a, int b) {return (a > b); });

			fin.close();
			ofstream fout("./Resource/Data/highscore.txt");

			count = count + 1 > 9 ? 9 : count + 1;
			fout << count << '\n';
			for (int i = 0; i < count; ++i) {
				fout << scores[i] << '\n';
			}
			fout.close();
		}
		else {
			ofstream fout("./Resource/Data/highscore.txt");
			fout << 1 << '\n';
			fout << score;
		}
	}

	
	OptionRequest Select(int key) {
		if (key != ENTER)
			return OptionRequest();
		switch (getSelectIndex()) {
		case 3:
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
		"Back to menu" };
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