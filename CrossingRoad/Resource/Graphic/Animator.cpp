#include "Animator.h"

void lefttrim(Frame& fr) {

	int mintrim = 999;
	for (int i = 0; i < fr.key.size(); i++) {
		for (int j = 0; j < fr.key[i].size(); j++) {
			if (fr.key[i][j] != '!' && j < mintrim) {
				mintrim = j;
				break;
			}
		}
	}
	for (int i = 0; i < fr.key.size(); i++) {
		if (!fr.key[i].empty() && mintrim > 0) {
			fr.key[i].erase(0, mintrim);
		}
	}
}
void righttrim(Frame& fr) {
	int longest = 0;
	int maxtrim = 0;
	for (int j = 0; j < fr.key.size(); j++) {
		if (fr.key[j].size() > longest) longest = fr.key[j].size();
	}
	for (int j = 0; j < fr.key.size(); j++) {
		while (fr.key[j].size() < longest) fr.key[j] += '!';
	}
	for (int i = 0; i < fr.key.size(); i++) {
		for (int j = fr.key[i].size() - 1; j >= 0; j--) {
			if (fr.key[i][j] != '!' && j > maxtrim) {
				maxtrim = j;
				break;
			}
		}
	}
	for (int j = 0; j < fr.key.size(); j++) {
		if (!fr.key[j].empty() && fr.key[j].size() > maxtrim) {
			fr.key[j].erase(maxtrim + 1, fr.key[j].size());
		}
	}
}
Frame revFrame(Frame fr) {
	for (int j = 0; j < fr.key.size(); j++) {
		reverse(fr.key[j].begin(), fr.key[j].end());
		for (int i = 0; i < fr.key[j].size(); i++) {
			if (fr.key[j][i] == '\\') fr.key[j][i] = '\/';
			else if (fr.key[j][i] == '\/') fr.key[j][i] = '\\';
			else if (fr.key[j][i] == ')') fr.key[j][i] = '(';
			else if (fr.key[j][i] == '(') fr.key[j][i] = ')';
		}
	}
	return fr;
}
void ReplaceAll(string& c, char f, char t)
{
	for (int i = 0; i < c.length(); i++) if (c[i] == f) c[i] = t;
}

Animator* readAnimator(string path, string sound, int id)
{
	ifstream info((string)ANIMATION + "/" + path);
	if (!info.is_open()) return nullptr;
	int speed, set, off_set_y;
	string k;
	info >> speed >> set >> off_set_y;
	info.ignore();
	vector<Frame> frame;
	vector<Frame> revframe;
	Frame kc;
	Frame rev;
	while (!info.eof()) {
		getline(info, k);
		ReplaceAll(k, ' ', '!');//Set background opacity to 0
		if (k == "X") {
			lefttrim(kc);
			righttrim(kc);
			rev = revFrame(kc);
			frame.push_back(kc);
			revframe.push_back(rev);
			kc = *new Frame();
			rev = *new Frame();
			continue;
		}
		kc.key.push_back(k);
	}
	return new Animator(path, frame, revframe, off_set_y, speed, id, set, sound);
}

Animator::Animator() = default;

Animator::Animator(string name,
	vector<Frame> & animation_set,
	vector<Frame> & animation_set_reverse,
	int off_set_y,
	int speed,
	int id,
	int set, string sound)
{
	this->name = name;
	this->animation_set = animation_set;
	this->animation_set_reverse = animation_set_reverse;
	this->id = id;
	this->set = set;
	this->speed = speed;
	this->off_set_y = off_set_y;
	this->sound = sound;
}

Animator::~Animator() = default;

int Animator::getWidth() {
	return animation_set[0].key[0].size();
}

int Animator::getOffSetY() {
	return off_set_y;
}

string Animator::getSound()
{
	return sound;
}

AnimatorData::AnimatorData() = default;

AnimatorData::AnimatorData(Animator * animator)
{
	this->animator = animator;
	reverse = false;
	k = 0;
	current_time = 0;
	refresh = 0;
	base = 0;
	lim = animator->animation_set.size() / animator->set;
}

void AnimatorData::push(int frame)
{
	current_time += frame;
	refresh += frame;
	if (current_time >= animator->speed) {
		current_time = 0;
		k++;
	}
}

void AnimatorData::changeAnimation(int i)
{
	base = i * animator->animation_set.size() / animator->set;
	lim = base + animator->animation_set.size() / animator->set;
	if (k < base && lim >= k) k = base;
	refresh = 0;
}

Frame AnimatorData::getFrame()
{
	if (k >= lim) {
		if (refresh > 50) {
			base = 0;
			lim = animator->animation_set.size() / animator->set;
		}
		k = base;
	}
	return reverse ? animator->animation_set_reverse[k] : animator->animation_set[k];
}

int AnimatorData::getWidth() {
	return getFrame().key[0].size();
}

int AnimatorData::getHeight() {
	return getFrame().key.size();
}
