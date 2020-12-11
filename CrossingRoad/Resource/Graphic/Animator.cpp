#include "Animator.h"

Frame trim(Frame fr) {
	int mintrim = 999;
	int maxtrim = 0;
	for (int i = 0; i < fr.key.size(); i++) {
		for (int j = 0; j < fr.key[i].size(); j++) {
			if (fr.key[i][j] != ' ' && j < mintrim) {
				mintrim = j;
				break;
			}
		}
		for (int j = fr.key[i].size() - 1; j >= 0; j--) {
			if (fr.key[i][j] != ' ' && j < maxtrim) {
				maxtrim = j;
				break;
			}
		}
	}
	for (int i = 0; i < fr.key.size(); i++) {
		fr.key[i].erase(0, mintrim - 1);
		fr.key[i].erase(maxtrim + 1, fr.key[i].size() - 1);
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
	Frame kc;
	int max = -1;
	while (!info.eof()) {
		getline(info, k);
		ReplaceAll(k, ' ', '!');//Set background opacity to 0
		if (max == -1 || max < k.length()) max = k.length();
		if (k == "X") {
			//trim(kc);
			frame.push_back(kc);
			kc = *new Frame();
			continue;
		}
		kc.key.push_back(k);
	}
	//trim(kc);
	return new Animator(frame, speed, id, max, set);
}