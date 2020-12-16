#include "Option.h"

OptionRequest::OptionRequest()
{
	id = -1;
}

OptionRequest::OptionRequest(int id, string value)
{
	this->id = id;
	this->value = value;
}

OptionRequest Option::Select(int key)
{
	return OptionRequest();
}

string* Option::getOptions()
{
	return s;
}

int Option::getLength()
{
	return length;
}

int Option::getSelectIndex()
{
	return select;
}

string Option::getTitle()
{
	return title;
}

void Option::Navigate(int k)
{
	select += k;
	if (select >= length) select = 0;
	else if (select < 0) select = length - 1;
}

MenuOption::MenuOption()
{
	title = "I need to take a break, can you help me guide these passengers?";
	length = 4;
	s = new string[length]{ "Play","Load game","Credit","Exit" };
	select = 0;
}

OptionRequest MenuOption::Select(int key)
{
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

LoadOption::LoadOption()
{
	title = "Type in your save's name";
	length = 2;
	s = new string[length]{ "", "Back" };
	select = 0;
}

OptionRequest LoadOption::Select(int key)
{
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

CreditOption::CreditOption()
{
	title = "Here are my creators: ";
	length = 6;
	s = new string[length]{ "Nguyen Lam Tuong - 19125017",
	"Truong Quy Hai - 19125090",
	"Tran Vinh Long - 19125134",
	"Nguyen Pham Tung Lam - 19125056",
	"",
	"Back" };
	select = 0;
}

OptionRequest CreditOption::Select(int key)
{
	if (key != ENTER)
		return OptionRequest();
	switch (getSelectIndex()) {
	case 5:
		return OptionRequest(BACK_MENU_REQUEST, "");
	}
	return OptionRequest();
}

LostOption::LostOption(int currentScore, int highScore)
{
	saveHighScore(currentScore);
	title = "Useless. Guess I have to do it myself >:(";
	length = 4;
	s = new string[length]{ "You scored: " + to_string(currentScore),
		"High score: " + to_string(highScore),
		"",
	"Back to menu" };
	select = 0;
}

void LostOption::saveHighScore(int score)
{
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

OptionRequest LostOption::Select(int key)
{
	if (key != ENTER)
		return OptionRequest();
	switch (getSelectIndex()) {
	case 3:
		return OptionRequest(BACK_MENU_REQUEST, "");
	}
	return OptionRequest();
}

PauseOption::PauseOption()
{
	saved = false;
	title = "Need to take a break?";
	length = 5;
	s = new string[length]{ "",
	"Save",
	"",
	"Resume",
	"Back to menu" };
	select = 0;
}

OptionRequest PauseOption::Select(int key)
{
	if (key != ENTER && getSelectIndex() == 0) {
		if (key == DELETE_KEY) s[0] = s[0].substr(0, s[0].length() - 1);
		else s[0] += (char)key;
		return OptionRequest();
	}
	switch (getSelectIndex()) {
	case 1:
		if (saved) break;
		saved = true;
		s[1] = "SAVE COMPLETE!";
		return OptionRequest(SAVE_SAVE_REQUEST, s[0]);
	case 3:
		return OptionRequest(RESUME_GAME, "");
	case 4:
		return OptionRequest(BACK_MENU_REQUEST, "");
	}
	return OptionRequest();
}
