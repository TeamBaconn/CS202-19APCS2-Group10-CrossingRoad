#include"Level.h"

LaneInfo::LaneInfo(int time) {
	this->time = time;
	toRight = rand() % 2;
	current_traffic = rand() % (TRAFFIC_TIME);
	open = new bool(rand() % 2);
}

int Level::getLane() const
{
	return lane - 1;
}

Level::Level(int lane, int mode)
{
	//Load resource
	ifstream info;
	info.open((string)ANIMATION + "/setting.txt");
	string c;
	string sound;
	int id;
	while (!info.eof()) {
		info >> id;
		info.ignore();
		getline(info, c);
		getline(info, sound);
		anim_list.push_back(readAnimator(c,sound, id));
	}

	this->lane = lane;
	this->mode = mode;
	this->width = LANE_WIDTH;
	this->height = lane * LANE_HEIGHT;

	//Spawn
	player = new Player
	(Position(LANE_WIDTH / 2, 2), getAnimator("human.txt"));
	entities.push_back(player);

	Animator* house = getAnimator("house.txt");
	entities.push_back(new Prop(Position(house->getWidth() / 2, 5), house));

	ResetLane();

	// quai vat initialize
	srand(time(NULL));
}

LaneInfo& Level::getLane(int i) {
	return SpawnArray[i];
}

void Level::PlaySoundEffect(string name)
{
	string path = (string)SOUND + name;
	std::wstring stemp = std::wstring(path.begin(), path.end());
	PlaySound(stemp.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void Level::ResetLane() {
	for (int i = 0; i < entities.size(); ++i)
		if (entities[i] != player && entities[i]->isHostile()) entities[i]->remove = true;

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
void Level::KillAllEntities() {
	int s = entities.size();
	for (int i = s - 1; i >= 0; --i) DeleteEntity(i);
}
void Level::resetLevel() {
	//Default
	lane = 5;
	mode = 2;
	lost = 0;
	checkPoint = 1;
	score = 0;

	KillAllEntities();

	player = new Player
	(Position(LANE_WIDTH / 2, 2), getAnimator("human.txt"));
	entities.push_back(player);

	Animator* house = getAnimator("house.txt");
	entities.push_back(new Prop(Position(house->getWidth() / 2, 5), house));

	ResetLane();
}
void Level::DeleteEntity(int index) {
	delete entities[index];
	entities.erase(entities.begin() + index);
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
	for (int i = s - 1; i >= 0; --i) if (entities[i]->remove) DeleteEntity(i);
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
			entities.push_back(new Hostile(p, e, i));
		}
	}
}

void Level::GameLose()
{
	if (lost) return;
	lost = 1;
	Animator* exp = getAnimator("explosion.txt");
	PlaySoundEffect(exp->getSound());
	player->render = false;
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
	vector<Entity*> newEntities;
	for (int i = 0; i < entities.size(); ++i) if (entities[i] && !entities[i]->remove) newEntities.push_back(entities[i]);
	return newEntities;
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

void Level::writeE2File(ofstream& fout, Entity* entity) {
	// animator
	fout << entity->getAnimatorData().getAnimator()->name << '\n';
	//ID
	fout << entity->getAnimatorData().getAnimator()->id << ' ';

	//Position
	fout << entity->pos.x << ' ';
	fout << entity->pos.y << ' ';

	// remove
	fout << entity->remove << '\n';
}
Animator* Level::getAnimator(string name) {
	for (int i = 0; i < anim_list.size(); i++) if (anim_list[i]->name == name) return anim_list[i];
	return nullptr;
}
void Level::readF2E(ifstream& fin, Entity*& entity) {
	string name;
	fin >> name;

	int id;
	fin >> id;

	Position pos;
	fin >> pos.x;
	fin >> pos.y;

	bool remove;
	fin >> remove;
	// animator
	Animator* temp = getAnimator(name);

	int lane = (pos.y-1) / LANE_HEIGHT - 1;
	if (id == HUMAN_ID) { entity = new Player(pos, temp); }
	else if (id == CAR_ID) { entity = new Hostile(pos, temp, lane); }
	else if (id == PROP_ID) { entity = new Prop(pos, temp); }
	else if (id == LIGHT_ID) { entity = new Light(pos, temp, lane); }

	entity->remove = remove;
}
void Level::saveLevel(string name) {
	ofstream fout("./Resource/Data/"+name+".txt");
	if (fout) {
		fout << lane << '\n';
		fout << mode << '\n';
		fout << score << '\n';

		for (int i = 0; i < SpawnArray.size(); i++) {
			fout << SpawnArray[i].toRight << ' ';
			fout << SpawnArray[i].open << ' ';
			fout << SpawnArray[i].current_traffic << ' ';
			fout << SpawnArray[i].time << '\n';
		}

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

		ResetLane();
		for (int i = 0; i < SpawnArray.size(); i++) {
			fin >> SpawnArray[i].toRight;
			fin >> SpawnArray[i].open;
			fin >> SpawnArray[i].current_traffic;
			fin >> SpawnArray[i].time;
		}

		size_t esize;
		fin >> esize;

		KillAllEntities();

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