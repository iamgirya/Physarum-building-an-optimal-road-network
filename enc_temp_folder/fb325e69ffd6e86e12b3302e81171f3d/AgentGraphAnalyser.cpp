#include "SlimeMoldClass.h"

AgentGraphAnalyser::AgentGraphAnalyser() {}

vector<pair<it, it>> AgentGraphAnalyser::makeMinimizedGraph(vector<SlimeAgent*> particles, vector<Generator*> generators) {
	vector<pair<it, it>> positions;
	for (auto u : particles) {
		positions.push_back(make_pair(u->pixelVector[0], u->pixelVector[1]));
	}
	sort(positions.begin(), positions.end());

	auto distance = [](pair<it, it> i, pair<it, it> j) { return sqrt(pow(i.first - j.first, 2) + pow(i.second - j.second, 2)); };
	auto average = [](pair<it, it> i, pair<it, it> j) {return make_pair((i.first+j.first)/2, (i.second + j.second) / 2); };

	//односвязный список рёбер. Чтобы удалять рёбра за 1
	struct EdgeListNode {
		it vextexOne;
		it vextexTwo;
		EdgeListNode* next = NULL;
		EdgeListNode* prev = NULL;
	};
	EdgeListNode* begin = new EdgeListNode();
	EdgeListNode* last = begin;
	int count = 0;

	for (int i = 0; i < positions.size(); i++) {
		for (int j = i+1; j < positions.size(); j++) {
			//благодаря сортировке из n^2 переходим в n*log(n). А в случае с нашими графами ускорение практически в 15 раз
			if (abs(positions[i].first - positions[j].first) > edgesRange) {
				break;
			}
			if (distance(positions[i], positions[j]) <= edgesRange) {
				EdgeListNode* newNode = new EdgeListNode();
				newNode->vextexOne = i;
				newNode->vextexTwo = j;
				newNode->prev = last;
				last->next = newNode;
				last = newNode;
				count++;
			}
		}
	}
	begin = begin->next;

	vector<it> teams(positions.size());
	for (int i = 0; i < positions.size(); i++) {
		teams[i] = i;
	}

	//фиктивный конец и начало
	last->next = new EdgeListNode();
	begin->prev = new EdgeListNode();
	bool hasChange = true;
	while (hasChange) {
		hasChange = false;
		last = begin;
		while (last->next != NULL) {
			EdgeListNode* next = last->next;
			if (teams[last->vextexOne] == teams[last->vextexTwo]) {
				last->prev->next = last->next;
				last->next->prev = last->prev;
				if (last == begin) {
					begin = begin->next;
				}
				delete last;
				count--;
			} else if (distance(positions[teams[last->vextexOne]], positions[teams[last->vextexTwo]]) <= vertexRange) {
				positions[teams[last->vextexOne]] = average(positions[teams[last->vextexOne]], positions[teams[last->vextexTwo]]);
				teams[last->vextexTwo] = teams[last->vextexOne];
				last->prev->next = last->next;
				last->next->prev = last->prev;
				if (last == begin) {
					begin = begin->next;
				}
				delete last;
				hasChange = true;
				count--;
			}
			last = next;
		}
	}
	begin->prev = NULL;

	set<it> tmpExitVertex;
	vector<it> exitVertex;
	vector<it> expexitVertex;
	for (int i = 0; i < positions.size(); i++) {
		tmpExitVertex.insert(teams[i]);
	}

	for (auto u: tmpExitVertex) {
		exitVertex.push_back(u);
	}

	vector<pair<it, it>> exitPoints;
	for (int i = 0; i < exitVertex.size(); i++) {
		exitPoints.push_back(positions[exitVertex[i]]);
	}

	/*vector<vector<it>> exitGraph(exitPoints.size(), vector<it>(exitPoints.size(), 0));
	last = begin;
	while (last != NULL) {
		exitGraph[teams[last->vextexOne]][teams[last->vextexOne]] = 1;
		last = last->next;
	}*/

	return exitPoints;
}