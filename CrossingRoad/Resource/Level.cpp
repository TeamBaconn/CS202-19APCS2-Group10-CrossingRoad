#include"Level.h"

LaneInfo::LaneInfo(int time) {
	this->time = time;
	toRight = rand() % 2;
	current_traffic = rand() % (TRAFFIC_TIME);
	open = new bool(rand() % 2);
}

Level::Level(int lane, int mode)
{
	//Load resource
	ifstream info;
	info.open((string)ANIMATION + "/setting.txt");
	string c;
	int id;
	while (!info.eof()) {
		info >> id;
		info.ignore();
		getline(info, c);
		anim_list.push_back(readAnimator(c, id));
	}

	this->lane = lane;
	this->mode = mode;
	this->width = LANE_WIDTH;
	this->height = lane * LANE_HEIGHT;

	//Spawn
	player = new Player
	(Position(LANE_WIDTH / 2, 2), getAnimation(HUMAN_ID)[0]);

	Animator* house = getAnimation(PROP_ID)[0];
	entities.push_back(new Prop(Position(house->getWidth() / 2, 5), house));

	ResetLane();

	// quai vat initialize
	srand(time(NULL));
	entities.push_back(player);
}

LaneInfo& Level::getLane(int i) {
	return SpawnArray[i];
}

void Level::ResetLane() {
	for (int i = 0; i < entities.size(); ++i)
		if (entities[i] != player && entities[i]->isCar()) entities[i]->remove = true;

	width = LANE_WIDTH;
	height = lane * LANE_HEIGHT;

	SpawnArray.clear();
	for (int i = 1; i < lane; i++) {
		LaneInfo info(0);
		SpawnArray.push_back(info);
		Position pos(info.toRight ? 3 : -3 + width, i * LANE_HEIGHT + LANE_HEIGHT * 4 / 5);
		entities.push_back(new Light(pos, getAnimation(LIGHT_ID)[0], i - 1));
	}
}

void Level::CheckEntity()
{
	for (int i = 0; i < entities.size(); i++)
		if (entities[i] != player) {
			if (entities[i]->Behavior(GAME_RATE, *this)) entities[i]->remove = true;
		}
		else if (entities[i] == player) {
			// if win delete all Car instances
			if (entities[i]->Behavior(GAME_RATE, *this)) {
				++mode;
				if (mode % 3)
					++lane;

				ResetLane();

				score += 20;
				checkPoint = 1;

				break;
			}
			else if (entities[i]->GetPosition().y > checkPoint * LANE_HEIGHT) {
				score += 10;
				++checkPoint;
			}
		}

	//This code to remove entity
	int s = entities.size();
	for (int i = s - 1; i >= 0; --i) {
		if (entities[i]->remove)
			entities.erase(entities.begin() + i);
	}
	spawnRandom();
}

void Level::spawnRandom()
{
	for (int i = 0; i < SpawnArray.size(); ++i) {
		SpawnArray[i].current_traffic += GAME_RATE;
		//Turn traffic light
		if (SpawnArray[i].current_traffic > TRAFFIC_TIME + SpawnArray[i].open * TRAFFIC_GAP) {
			SpawnArray[i].open = !SpawnArray[i].open;
			SpawnArray[i].current_traffic = 0;
		}
		//Check if traffic light is open then start spawning time
		if (SpawnArray[i].open) SpawnArray[i].time -= GAME_RATE;
		//Spawn entity
		if (SpawnArray[i].time < 0) {
			SpawnArray[i].time = MIN_SPAWN_TIME + rand() % (SPAN_TIME / mode);
			vector<Animator*> anim = getAnimation(CAR_ID);

			Animator* e = anim[rand() % anim.size()];

			Position p;
			if (!SpawnArray[i].toRight) p = Position(INGAME_WIDTH + e->getWidth(), 0);
			p = p + Position(0, (i + 1) * LANE_HEIGHT + LANE_HEIGHT*1/2+ (rand()%(LANE_HEIGHT*2/3)));
			entities.push_back(new Car(p, e, i));
		}
	}
}

void Level::LooseGame()
{
	if (lost) return;
	lost = 1;
	Animator* exp = getAnimation(PROP_ID)[1];
	player->remove = true;
	Entity* en = new Prop(Position(exp->getWidth() / 2 + player->GetPosition().x, player->GetPosition().y), exp);
	en->changeBase(1);
	entities.push_back(en);
}

const Entity* Level::getPlayer() const
{
	return player;
}

vector<Animator*> Level::getAnimation(int type) const
{
	vector<Animator*> list;
	for (int i = 0; i < anim_list.size(); i++)
		if (anim_list[i]->id == type) list.emplace_back(anim_list[i]);
	return list;
}

char** Level::generateMap(int width, int height) const
{
	char** map = reset(width, height);
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
				map[i][j] = '#';
	for (int i = 1; i < lane; i++)
		for (int j = 0; j < width; j += LANE_DISTANCE) map[j][i * LANE_HEIGHT] = '-';
	return map;
}

char** Level::reset(int width, int height)
{
	char** map = new char* [width];
	for (int i = 0; i < width; ++i) {
		map[i] = new char[height];
		for (int j = 0; j < height; j++)
			map[i][j] = ' ';
	}
	return map;
}

void Level::deleteMap(char**& old, int width)
{
	if (old == nullptr) return;
	for (int i = 0; i < width; i++) {
		delete[] old[i];
		old[i] = nullptr;
	}
	delete[] old;
	old = nullptr;
}

vector<Entity*> Level::getEntities() const
{
	return entities;
}

int Level::getWidth() const
{
	return width;
}

int Level::getHeight() const
{
	return height;
}

int Level::getScore() const
{
	return score;
}

void Level::outputAnimation(vector<Frame>& set, ofstream& fout, Entity* entity) {
	int sSize = set.size();
	fout << sSize << '\n';
	for (int i = 0; i < sSize; ++i) {
		int kSize = set[i].key.size();
		fout << kSize << '\n';
		for (int j = 0; j < kSize; ++j) {
			fout << set[i].key[j] << '\n';
		}
	}
}
void Level::writeE2File(ofstream& fout, Entity* entity) {
	int id = entity->getID();
	fout << id << '\n';

	fout << entity->pos.x << '\n';
	fout << entity->pos.y << '\n';

	// animator
	fout << entity->animator->id << '\n';
	fout << entity->animator->set << '\n';
	fout << entity->animator->speed << '\n';

	//
	vector<Frame> set = entity->animator->animation_set;
	outputAnimation(set, fout, entity);

	set = entity->animator->animation_set_reverse;
	outputAnimation(set, fout, entity);
	// animator

	fout << entity->remove << '\n';
}
void Level::inputAnimation(ifstream& fin, vector<Frame>& animation_set) {
	Frame keys;

	int sSize;
	fin >> sSize;
	for (int i = 0; i < sSize; ++i) {
		int kSize;
		fin >> kSize;
		fin.ignore(123, '\n');
		for (int j = 0; j < kSize; ++j) {
			string key;
			getline(fin, key);
			keys.key.push_back(key);
		}
		animation_set.push_back(keys);
		keys.key.clear();
	}
}
void Level::readF2E(ifstream& fin, Entity*& entity) {
	int id;
	fin >> id;

	Position pos;
	fin >> pos.x;
	fin >> pos.y;

	// animator
	Animator* temp = new Animator;

	fin >> temp->id;
	fin >> temp->set;
	fin >> temp->speed;

	vector<Frame> animation_set;
	//
	inputAnimation(fin, animation_set);
	temp->animation_set = animation_set;

	vector<Frame> animation_set_reverse;
	inputAnimation(fin, animation_set_reverse);
	temp->animation_set_reverse = animation_set_reverse;

	int lane = pos.y / LANE_HEIGHT - 1;
	if (id == HUMAN_ID) { entity = new Player(pos, temp); }
	else if (id == CAR_ID) { entity = new Car(pos, temp, lane); }
	else if (id == PROP_ID) { entity = new Prop(pos, temp); }
	else if (id == LIGHT_ID) { entity = new Light(pos, temp, lane); }

	fin >> entity->remove;
}
void Level::saveLevel(string name) {
	ofstream fout("./Resource/Data/"+name+".txt");
	if (fout) {
		fout << lane << '\n';
		fout << mode << '\n';
		fout << score << '\n';

		size_t esize = entities.size();
		fout << esize << '\n';
		writeE2File(fout, player);
		for (int i = 0; i < esize; ++i) {
			if (entities[i] != player) {
				writeE2File(fout, entities[i]);
			}
		}

		fout.close();
	}
}
void Level::loadLevel(string name) {
	ifstream fin("./Resource/Data/" + name + ".txt");
	if (fin) {
		fin >> lane;
		fin >> mode;
		fin >> score;
		height = lane * LANE_HEIGHT;

		size_t esize;
		fin >> esize;
		entities.clear();

		Entity* temp = nullptr;
		readF2E(fin, temp);
		entities.push_back(temp);
		player = temp;
		for (int i = 1; i < esize; ++i) {
			readF2E(fin, temp);
			entities.push_back(temp);
		}

		fin.close();
	}
}