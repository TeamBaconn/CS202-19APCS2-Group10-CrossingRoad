#ifndef ANIMATOR_H_INCLUDED
#define ANIMATOR_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define ANIMATION "./Resource/Animation/"
const std::string WHITESPACE = " \n\r\t\f\v";

using namespace std;

struct Frame {
	vector<string> key;
};
class Animator {
private:
	string name,sound;
	int id, set, speed, off_set_y;
	vector<Frame> animation_set,animation_set_reverse;
public:
	Animator();
	Animator(string name, vector<Frame>& animation_set, vector<Frame>& animation_set_reverse, int off_set_y, int speed, int id, int set,string sound);
	~Animator();

	int getWidth();
	int getOffSetY();
	string getSound();
	friend class Level;
	friend class AnimatorData;
};
class AnimatorData {
private:
	int k, current_time, base, lim, refresh;
	Animator* animator;
public:
	bool reverse;
	AnimatorData();
	AnimatorData(Animator* animator);
	void push(int frame);
	void changeAnimation(int i);
	Frame getFrame();
	int getWidth();
	int getHeight();

	Animator* getAnimator() {
		return animator;
	}
};
void ReplaceAll(string& c, char f, char t);
Animator* readAnimator(string path,string sound, int id);
#endif // ANIMATOR_H_INCLUDED