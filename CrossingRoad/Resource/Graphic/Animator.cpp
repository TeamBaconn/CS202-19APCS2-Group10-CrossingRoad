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
			fr.key[j].erase(maxtrim+1 , fr.key[j].size());
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

Animator* readAnimator(string path, int id)
{
	ifstream info((string)ANIMATION + "/" + path);
	if (!info.is_open()) return nullptr;
	int speed, set;
	string k;
	info >> speed >> set;
	info.ignore();
	vector<Frame> frame;
	vector<Frame> revframe;
	Frame kc;
	Frame rev;
	int max = -1;
	while (!info.eof()) {
		getline(info, k);
		ReplaceAll(k, ' ', '!');//Set background opacity to 0
		if (max == -1 || max < k.length()) max = k.length();
		if (k == "X") {
			lefttrim(kc);
			righttrim(kc);
			rev =revFrame(kc);
			frame.push_back(kc);
			revframe.push_back(rev);
			kc = *new Frame();
			rev = *new Frame();
			continue;
		}
		kc.key.push_back(k);
	}
	//trim(kc);
	return new Animator(frame, revframe, speed, id, max, set);
}