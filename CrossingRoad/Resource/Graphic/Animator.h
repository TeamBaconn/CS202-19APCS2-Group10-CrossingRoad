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
	int id, width, set, speed;
	vector<Frame> animation_set;
public:
	Animator() = default;
	Animator(vector<Frame>& animation_set, int speed, int id, int width, int set) {
		this->width = width;
		this->animation_set = animation_set;
		this->id = id;
		this->set = set;
		this->speed = speed;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return animation_set[0].key.size();
	}
	~Animator() = default;

	friend class Level;
	friend class AnimatorData;
};
class AnimatorData {
private:
	int k, current_time, base, lim, refresh;
	Animator* animator;
public:
	AnimatorData() = default;
	AnimatorData(Animator* animator) {
		this->animator = animator;
		k = 0;
		current_time = 0;
		refresh = 0;
		base = 0;
		lim = animator->animation_set.size() / animator->set;
	}
	void push(int frame) {
		current_time += frame;
		refresh += frame;
		if (current_time >= animator->speed) {
			current_time = 0;
			k++;
		}
	}
	void changeAnimation(int i) {
		base = i * animator->animation_set.size() / animator->set;
		lim = base + animator->animation_set.size() / animator->set;
		if (k < base && lim >= k) k = base;
		refresh = 0;
	}
	Frame getFrame() {
		if (k >= lim) {
			if (refresh > 50) {
				base = 0;
				lim = animator->animation_set.size() / animator->set;
			}
			k = base;
		}
		return animator->animation_set[k];
	}
};
void ReplaceAll(string& c, char f, char t);
Animator* readAnimator(string path, int id);
#endif // ANIMATOR_H_INCLUDED